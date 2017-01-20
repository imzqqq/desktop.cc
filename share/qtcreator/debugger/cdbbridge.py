############################################################################
#
# Copyright (C) 2016 The Qt Company Ltd.
# Contact: https://www.qt.io/licensing/
#
# This file is part of Qt Creator.
#
# Commercial License Usage
# Licensees holding valid commercial Qt licenses may use this file in
# accordance with the commercial license agreement provided with the
# Software or, alternatively, in accordance with the terms contained in
# a written agreement between you and The Qt Company. For licensing terms
# and conditions see https://www.qt.io/terms-conditions. For further
# information use the contact form at https://www.qt.io/contact-us.
#
# GNU General Public License Usage
# Alternatively, this file may be used under the terms of the GNU
# General Public License version 3 as published by the Free Software
# Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
# included in the packaging of this file. Please review the following
# information to ensure the GNU General Public License requirements will
# be met: https://www.gnu.org/licenses/gpl-3.0.html.
#
############################################################################

import inspect
import os
import sys
import cdbext
import re

sys.path.insert(1, os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe()))))

from dumper import *

class FakeVoidType(cdbext.Type):
    def __init__(self, name , dumper):
        cdbext.Type.__init__(self)
        self.typeName = name.strip()
        self.dumper = dumper

    def name(self):
        return self.typeName

    def bitsize(self):
        return self.dumper.ptrSize() * 8

    def code(self):
        if self.typeName.endswith('*'):
            return TypeCodePointer
        if self.typeName.endswith(']'):
            return TypeCodeArray
        return TypeCodeVoid

    def unqualified(self):
        return self

    def target(self):
        code = self.code()
        if code == TypeCodePointer:
            return FakeVoidType(self.typeName[:-1], self.dumper)
        if code == TypeCodeVoid:
            return self
        try:
            return FakeVoidType(self.typeName[:self.typeName.rindex('[')], self.dumper)
        except:
            return FakeVoidType('void', self.dumper)

    def stripTypedef(self):
        return self

    def fields(self):
        return []

    def templateArgument(self, pos, numeric):
        return None

    def templateArguments(self):
        return []

class Dumper(DumperBase):
    def __init__(self):
        DumperBase.__init__(self)
        self.outputLock = threading.Lock()
        self.isCdb = True

    def fromNativeValue(self, nativeValue):
        self.check(isinstance(nativeValue, cdbext.Value))
        val = self.Value(self)
        val.name = nativeValue.name()
        val.type = self.fromNativeType(nativeValue.type())
        # There is no cdb api for the size of bitfields.
        # Workaround this issue by parsing the native debugger text for integral types.
        if val.type.code == TypeCodeIntegral:
            integerString = nativeValue.nativeDebuggerValue()
            if integerString == 'true':
                val.ldata = int(1).to_bytes(1, byteorder='little')
            elif integerString == 'false':
                val.ldata = int(0).to_bytes(1, byteorder='little')
            else:
                integerString = integerString.replace('`','')
                integerString = integerString.split(' ')[0]
                if integerString.startswith('0n'):
                    integerString = integerString[2:]
                    base = 10
                elif integerString.startswith('0x'):
                    base = 16
                else:
                    base = 10
                signed = not val.type.name.startswith('unsigned')
                val.ldata = int(integerString, base).to_bytes(val.type.size(), \
                        byteorder='little', signed=signed)
        val.isBaseClass = val.name == val.type.name
        val.lIsInScope = True
        val.laddress = nativeValue.address()
        return val

    def nativeTypeId(self, nativeType):
        self.check(isinstance(nativeType, cdbext.Type))
        name = nativeType.name()
        if name is None or len(name) == 0:
            c = '0'
        elif name == 'struct {...}':
            c = 's'
        elif name == 'union {...}':
            c = 'u'
        else:
            return name
        typeId = c + ''.join(['{%s:%s}' % (f.name(), self.nativeTypeId(f.type())) for f in nativeType.fields()])
        return typeId

    def fromNativeType(self, nativeType):
        self.check(isinstance(nativeType, cdbext.Type))
        code = nativeType.code()

        if nativeType.name().startswith('void'):
            nativeType = FakeVoidType(nativeType.name(), self)

        if code == TypeCodePointer:
            return self.createPointerType(self.fromNativeType(nativeType.target()))

        if code == TypeCodeArray:
            targetType = self.fromNativeType(nativeType.target())
            return self.createArrayType(targetType, nativeType.arrayElements())

        typeId = self.nativeTypeId(nativeType)
        if self.typeData.get(typeId, None) is None:
            tdata = self.TypeData(self)
            tdata.name = nativeType.name()
            tdata.typeId = typeId
            tdata.lbitsize = nativeType.bitsize()
            tdata.code = code
            self.registerType(typeId, tdata) # Prevent recursion in fields.
            if  code == TypeCodeStruct:
                tdata.lfields = lambda value : \
                    self.listFields(nativeType, value)
                tdata.lalignment = lambda : \
                    self.nativeStructAlignment(nativeType)
            tdata.templateArguments = self.listTemplateParameters(nativeType)
            self.registerType(typeId, tdata) # Fix up fields and template args
        return self.Type(self, typeId)

    def listFields(self, nativeType, value):
        if value.address() is None or value.address() == 0:
            raise Exception("")
        nativeValue = cdbext.createValue(value.address(), nativeType)
        index = 0
        nativeMember = nativeValue.childFromIndex(index)
        while nativeMember is not None:
            yield self.fromNativeValue(nativeMember)
            index += 1
            nativeMember = nativeValue.childFromIndex(index)

    def nativeStructAlignment(self, nativeType):
        #warn("NATIVE ALIGN FOR %s" % nativeType.name)
        def handleItem(nativeFieldType, align):
            a = self.fromNativeType(nativeFieldType).alignment()
            return a if a > align else align
        align = 1
        for f in nativeType.fields():
            align = handleItem(f.type(), align)
        return align

    def listTemplateParameters(self, nativeType):
        targs = []
        for targ in nativeType.templateArguments():
            if isinstance(targ, str):
                if self.typeData.get(targ, None) is None:
                    targs.append(self.lookupType(targ))
                else:
                    targs.append(self.Type(self, targ))
            elif isinstance(targ, int):
                targs.append(targ)
            else:
                error('CDBCRAP %s' % type(targ))
        return targs

    def nativeTypeEnumDisplay(self, nativeType, intval):
        # TODO: generate fake value
        return None

    def enumExpression(self, enumType, enumValue):
        ns = self.qtNamespace()
        return ns + "Qt::" + enumType + "(" \
            + ns + "Qt::" + enumType + "::" + enumValue + ")"

    def pokeValue(self, typeName, *args):
        return None

    def parseAndEvaluate(self, exp):
        val = cdbext.parseAndEvaluate(exp)
        if val is None:
            return None
        value = self.Value(self)
        value.type = self.lookupType('void *')
        value.ldata = val.to_bytes(8, sys.byteorder)
        return value

    def isWindowsTarget(self):
        return True

    def isQnxTarget(self):
        return False

    def isArmArchitecture(self):
        return False

    def isMsvcTarget(self):
        return True

    def qtCoreModuleName(self):
        modules = cdbext.listOfModules()
        # first check for an exact module name match
        for coreName in ['Qt5Cored', 'Qt5Core', 'QtCored4', 'QtCore4']:
            if coreName in modules:
                self.qtCoreModuleName = lambda: coreName
                return coreName
        # maybe we have a libinfix build.
        for pattern in ['Qt5Core.*', 'QtCore.*']:
            matches = [module for module in modules if re.match(pattern, module)]
            if matches:
                coreName = matches[0]
                self.qtCoreModuleName = lambda: coreName
                return coreName
        return None

    def qtDeclarativeModuleName(self):
        modules = cdbext.listOfModules()
        for declarativeModuleName in ['Qt5Qmld', 'Qt5Qml']:
            if declarativeModuleName in modules:
                self.qtDeclarativeModuleName = lambda: declarativeModuleName
                return declarativeModuleName
        matches = [module for module in modules if re.match('Qt5Qml.*', module)]
        if matches:
            declarativeModuleName = matches[0]
            self.qtDeclarativeModuleName = lambda: declarativeModuleName
            return declarativeModuleName
        return None

    def qtHookDataSymbolName(self):
        hookSymbolName = 'qtHookData'
        coreModuleName = self.qtCoreModuleName()
        if coreModuleName is not None:
            hookSymbolName = '%s!%s%s' % (coreModuleName, self.qtNamespace(), hookSymbolName)
        else:
            resolved = cdbext.resolveSymbol('*' + hookSymbolName)
            if resolved:
                hookSymbolName = resolved[0]
            else:
                hookSymbolName = '*%s' % hookSymbolName
        self.qtHookDataSymbolName = lambda: hookSymbolName
        return hookSymbolName

    def qtDeclarativeHookDataSymbolName(self):
        hookSymbolName = 'qtDeclarativeHookData'
        declarativeModuleName = self.qtDeclarativeModuleName()
        if declarativeModuleName is not None:
            hookSymbolName = '%s!%s%s' % (declarativeModuleName, self.qtNamespace(), hookSymbolName)
        else:
            resolved = cdbext.resolveSymbol('*' + hookSymbolName)
            if resolved:
                hookSymbolName = resolved[0]
            else:
                hookSymbolName = '*%s' % hookSymbolName

        self.qtDeclarativeHookDataSymbolName = lambda: hookSymbolName
        return hookSymbolName

    def qtNamespace(self):
        namespace = ''
        qstrdupSymbolName = '*qstrdup'
        coreModuleName = self.qtCoreModuleName()
        if coreModuleName is not None:
            qstrdupSymbolName = '%s!%s' % (coreModuleName, qstrdupSymbolName)
        resolved = cdbext.resolveSymbol(qstrdupSymbolName)
        if resolved:
            name = resolved[0].split('!')[1]
            namespaceIndex = name.find('::')
            if namespaceIndex > 0:
                namespace = name[:namespaceIndex + 2]
        self.qtNamespace = lambda: namespace
        self.qtCustomEventFunc = cdbext.parseAndEvaluate('%s!%sQObject::customEvent'
                                                         % (self.qtCoreModuleName(), namespace))
        return namespace

    def couldBeQObjectVTable(self, vtablePtr):
        try:
            customEventFunc = self.extractPointer(vtablePtr + 8 * self.ptrSize())
        except:
            self.bump('nostruct-3')
            return False

        if customEventFunc in (self.qtCustomEventFunc, self.qtCustomEventPltFunc):
            return True
        try:
            delta = int.from_bytes(self.readRawMemory(customEventFunc + 1, 4), byteorder='little')
            if (customEventFunc + 5 + delta) in (self.qtCustomEventFunc, self.qtCustomEventPltFunc):
                return True
        except:
            pass

        try:
            return 'QObject::customEvent' in cdbext.getNameByAddress(customEventFunc)
        except:
            return False


    def qtVersion(self):
        qtVersion = self.findValueByExpression('((void**)&%s)[2]' % self.qtHookDataSymbolName())
        if qtVersion is None and self.qtCoreModuleName() is not None:
            try:
                versionValue = cdbext.call(self.qtCoreModuleName() + '!qVersion()')
                version = self.extractCString(self.fromNativeValue(versionValue).address())
                (major, minor, patch) = version.decode('latin1').split('.')
                qtVersion = 0x10000 * int(major) + 0x100 * int(minor) + int(patch)
            except:
                pass
        if qtVersion is None:
            qtVersion = self.fallbackQtVersion
        self.qtVersion = lambda: qtVersion
        return qtVersion

    def putVtableItem(self, address):
        funcName = cdbext.getNameByAddress(address)
        if funcName is None:
            self.putItem(self.createPointerValue(address, 'void'))
        else:
            self.putValue(funcName)
            self.putType('void*')
            self.putAddress(address)

    def putVTableChildren(self, item, itemCount):
        p = item.address()
        for i in xrange(itemCount):
            deref = self.extractPointer(p)
            if deref == 0:
                n = i
                break
            with SubItem(self, i):
                self.putVtableItem(deref)
                p += self.ptrSize()
        return itemCount

    def ptrSize(self):
        size = cdbext.pointerSize()
        self.ptrSize = lambda: size
        return size

    def put(self, stuff):
        self.output += stuff

    def lookupType(self, typeName):
        if len(typeName) == 0:
            return None
        if self.typeData.get(typeName, None) is None:
            nativeType = self.lookupNativeType(typeName)
            return None if nativeType is None else self.fromNativeType(nativeType)
        return self.Type(self, typeName)

    def lookupNativeType(self, name):
        if name.startswith('void'):
            return FakeVoidType(name, self)
        return cdbext.lookupType(name)

    def reportResult(self, result, args):
        self.report('result={%s}' % (result))

    def readRawMemory(self, address, size):
        mem = cdbext.readRawMemory(address, size)
        if len(mem) != size:
            raise Exception("Invalid memory request")
        return mem

    def findStaticMetaObject(self, typeName):
        ptr = self.findValueByExpression('&' + typeName + '::staticMetaObject')
        return ptr

    def warn(self, msg):
        self.put('{name="%s",value="",type="",numchild="0"},' % msg)

    def fetchVariables(self, args):
        self.resetStats()
        (ok, res) = self.tryFetchInterpreterVariables(args)
        if ok:
            self.reportResult(res, args)
            return

        self.setVariableFetchingOptions(args)

        self.output = ''

        self.currentIName = 'local'
        self.put('data=[')
        self.anonNumber = 0

        variables = []
        for val in cdbext.listOfLocals(self.partialVariable):
            variables.append(self.fromNativeValue(val))

        self.handleLocals(variables)
        self.handleWatches(args)

        self.put('],partial="%d"' % (len(self.partialVariable) > 0))
        self.put(',timings=%s' % self.timings)
        self.reportResult(self.output, args)

    def report(self, stuff):
        sys.stdout.write(stuff + "\n")

    def loadDumpers(self, args):
        msg = self.setupDumpers()
        self.reportResult(msg, args)

    def findValueByExpression(self, exp):
        return cdbext.parseAndEvaluate(exp)

    def nativeDynamicTypeName(self, address, baseType):
        return None # FIXME: Seems sufficient, no idea why.

    def callHelper(self, rettype, value, function, args):
        raise Exception("cdb does not support calling functions")

    def putCallItem(self, name, rettype, value, func, *args):
        return
