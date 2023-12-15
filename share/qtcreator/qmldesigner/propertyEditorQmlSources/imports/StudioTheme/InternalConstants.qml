// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

import QtQuick

QtObject {
    readonly property int width: 1920
    readonly property int height: 1080

    property FontLoader controlIcons: FontLoader {
        source: "icons.ttf"
    }

    objectName: "internalConstantsObject"

    readonly property string actionIcon: "\u0021"
    readonly property string actionIconBinding: "\u0022"
    readonly property string addColumnAfter: "\u0023"
    readonly property string addColumnBefore: "\u0024"
    readonly property string addFile: "\u0025"
    readonly property string addGroup_medium: "\u0026"
    readonly property string addRowAfter: "\u0027"
    readonly property string addRowBefore: "\u0028"
    readonly property string addTable: "\u0029"
    readonly property string add_medium: "\u002A"
    readonly property string add_small: "\u002B"
    readonly property string addcolumnleft_medium: "\u002C"
    readonly property string addcolumnright_medium: "\u002D"
    readonly property string addrowabove_medium: "\u002E"
    readonly property string addrowbelow_medium: "\u002F"
    readonly property string adsClose: "\u0030"
    readonly property string adsDetach: "\u0031"
    readonly property string adsDropDown: "\u0032"
    readonly property string alias: "\u0033"
    readonly property string aliasAnimated: "\u0034"
    readonly property string alignBottom: "\u0035"
    readonly property string alignCenterHorizontal: "\u0036"
    readonly property string alignCenterVertical: "\u0037"
    readonly property string alignLeft: "\u0038"
    readonly property string alignRight: "\u0039"
    readonly property string alignTo: "\u003A"
    readonly property string alignToCam_medium: "\u003B"
    readonly property string alignToCamera_small: "\u003C"
    readonly property string alignToObject_small: "\u003D"
    readonly property string alignToView_medium: "\u003E"
    readonly property string alignTop: "\u003F"
    readonly property string alphabetical_medium: "\u0040"
    readonly property string anchorBaseline: "\u0041"
    readonly property string anchorBottom: "\u0042"
    readonly property string anchorFill: "\u0043"
    readonly property string anchorLeft: "\u0044"
    readonly property string anchorRight: "\u0045"
    readonly property string anchorTop: "\u0046"
    readonly property string anchors_small: "\u0047"
    readonly property string animatedProperty: "\u0048"
    readonly property string annotationBubble: "\u0049"
    readonly property string annotationDecal: "\u004A"
    readonly property string annotations_large: "\u004B"
    readonly property string annotations_small: "\u004C"
    readonly property string applyMaterialToSelected: "\u004D"
    readonly property string apply_medium: "\u004E"
    readonly property string apply_small: "\u004F"
    readonly property string arrange_small: "\u0050"
    readonly property string arrow_small: "\u0051"
    readonly property string assign: "\u0052"
    readonly property string assignTo: "\u0053"
    readonly property string assignTo_medium: "\u0054"
    readonly property string attach_medium: "\u0055"
    readonly property string back_medium: "\u0056"
    readonly property string backspace_small: "\u0057"
    readonly property string bakeLights_medium: "\u0058"
    readonly property string bevelAll: "\u0059"
    readonly property string bevelCorner: "\u005A"
    readonly property string bezier_medium: "\u005B"
    readonly property string binding_medium: "\u005C"
    readonly property string bounds_small: "\u005D"
    readonly property string branch_medium: "\u005E"
    readonly property string camera_small: "\u005F"
    readonly property string centerHorizontal: "\u0060"
    readonly property string centerVertical: "\u0061"
    readonly property string cleanLogs_medium: "\u0062"
    readonly property string clearList_large: "\u0063"
    readonly property string clearList_medium: "\u0064"
    readonly property string closeCross: "\u0065"
    readonly property string closeFile_large: "\u0066"
    readonly property string closeLink: "\u0067"
    readonly property string close_small: "\u0068"
    readonly property string code: "\u0069"
    readonly property string codeEditor_medium: "\u006A"
    readonly property string codeview_medium: "\u006B"
    readonly property string colorPopupClose: "\u006C"
    readonly property string colorSelection_medium: "\u006D"
    readonly property string columnsAndRows: "\u006E"
    readonly property string comboBox_medium: "\u006F"
    readonly property string cone_medium: "\u0070"
    readonly property string cone_small: "\u0071"
    readonly property string connection_small: "\u0072"
    readonly property string connections_medium: "\u0073"
    readonly property string copyLink: "\u0074"
    readonly property string copyStyle: "\u0075"
    readonly property string copy_small: "\u0076"
    readonly property string cornerA: "\u0077"
    readonly property string cornerB: "\u0078"
    readonly property string cornersAll: "\u0079"
    readonly property string createComponent_large: "\u007A"
    readonly property string createComponent_small: "\u007B"
    readonly property string createObject_medium: "\u007C"
    readonly property string create_medium: "\u007D"
    readonly property string create_small: "\u007E"
    readonly property string cube_medium: "\u007F"
    readonly property string cube_small: "\u0080"
    readonly property string curveDesigner: "\u0081"
    readonly property string curveDesigner_medium: "\u0082"
    readonly property string curveEditor: "\u0083"
    readonly property string customMaterialEditor: "\u0084"
    readonly property string cylinder_medium: "\u0085"
    readonly property string cylinder_small: "\u0086"
    readonly property string decisionNode: "\u0087"
    readonly property string deleteColumn: "\u0088"
    readonly property string deleteMaterial: "\u0089"
    readonly property string deleteRow: "\u008A"
    readonly property string deleteTable: "\u008B"
    readonly property string delete_medium: "\u008C"
    readonly property string delete_small: "\u008D"
    readonly property string deletecolumn_medium: "\u008E"
    readonly property string deletepermanently_medium: "\u008F"
    readonly property string deleterow_medium: "\u0090"
    readonly property string designMode_large: "\u0091"
    readonly property string detach: "\u0092"
    readonly property string directionalLight_small: "\u0093"
    readonly property string distributeBottom: "\u0094"
    readonly property string distributeCenterHorizontal: "\u0095"
    readonly property string distributeCenterVertical: "\u0096"
    readonly property string distributeLeft: "\u0097"
    readonly property string distributeOriginBottomRight: "\u0098"
    readonly property string distributeOriginCenter: "\u0099"
    readonly property string distributeOriginNone: "\u009A"
    readonly property string distributeOriginTopLeft: "\u009B"
    readonly property string distributeRight: "\u009D"
    readonly property string distributeSpacingHorizontal: "\u009E"
    readonly property string distributeSpacingVertical: "\u009F"
    readonly property string distributeTop: "\u00A0"
    readonly property string download: "\u00A1"
    readonly property string downloadUnavailable: "\u00A2"
    readonly property string downloadUpdate: "\u00A3"
    readonly property string downloaded: "\u00A4"
    readonly property string dragmarks: "\u00A5"
    readonly property string duplicate_small: "\u00A6"
    readonly property string edit: "\u00A7"
    readonly property string editComponent_large: "\u00A8"
    readonly property string editComponent_small: "\u00A9"
    readonly property string editLightOff_medium: "\u00AA"
    readonly property string editLightOn_medium: "\u00AB"
    readonly property string edit_medium: "\u00AC"
    readonly property string edit_small: "\u00AE"
    readonly property string effects: "\u00AF"
    readonly property string events_small: "\u00B0"
    readonly property string export_medium: "\u00B1"
    readonly property string eyeDropper: "\u00B2"
    readonly property string favorite: "\u00B3"
    readonly property string fitAll_medium: "\u00B4"
    readonly property string fitSelected_small: "\u00B5"
    readonly property string fitSelection_medium: "\u00B6"
    readonly property string fitToView_medium: "\u00B7"
    readonly property string flowAction: "\u00B8"
    readonly property string flowTransition: "\u00B9"
    readonly property string fontStyleBold: "\u00BA"
    readonly property string fontStyleItalic: "\u00BB"
    readonly property string fontStyleStrikethrough: "\u00BC"
    readonly property string fontStyleUnderline: "\u00BD"
    readonly property string forward_medium: "\u00BE"
    readonly property string globalOrient_medium: "\u00BF"
    readonly property string gradient: "\u00C0"
    readonly property string gridView: "\u00C1"
    readonly property string grid_medium: "\u00C2"
    readonly property string group_small: "\u00C3"
    readonly property string help: "\u00C4"
    readonly property string home_large: "\u00C5"
    readonly property string idAliasOff: "\u00C6"
    readonly property string idAliasOn: "\u00C7"
    readonly property string import_medium: "\u00C8"
    readonly property string imported: "\u00C9"
    readonly property string importedModels_small: "\u00CA"
    readonly property string infinity: "\u00CB"
    readonly property string invisible_medium: "\u00CC"
    readonly property string invisible_small: "\u00CD"
    readonly property string jumpToCode_medium: "\u00CE"
    readonly property string jumpToCode_small: "\u00CF"
    readonly property string keyframe: "\u00D0"
    readonly property string languageList_medium: "\u00D1"
    readonly property string layouts_small: "\u00D2"
    readonly property string lights_small: "\u00D3"
    readonly property string linear_medium: "\u00D4"
    readonly property string linkTriangle: "\u00D5"
    readonly property string linked: "\u00D6"
    readonly property string listView: "\u00D7"
    readonly property string listView_medium: "\u00D8"
    readonly property string list_medium: "\u00D9"
    readonly property string localOrient_medium: "\u00DA"
    readonly property string lockOff: "\u00DB"
    readonly property string lockOn: "\u00DC"
    readonly property string loopPlayback_medium: "\u00DD"
    readonly property string materialBrowser_medium: "\u00DE"
    readonly property string materialPreviewEnvironment: "\u00DF"
    readonly property string materialPreviewModel: "\u00E0"
    readonly property string material_medium: "\u00E1"
    readonly property string maxBar_small: "\u00E2"
    readonly property string mergeCells: "\u00E3"
    readonly property string merge_small: "\u00E4"
    readonly property string minus: "\u00E5"
    readonly property string mirror: "\u00E6"
    readonly property string more_medium: "\u00E7"
    readonly property string mouseArea_small: "\u00E8"
    readonly property string moveDown_medium: "\u00E9"
    readonly property string moveInwards_medium: "\u00EA"
    readonly property string moveUp_medium: "\u00EB"
    readonly property string moveUpwards_medium: "\u00EC"
    readonly property string move_medium: "\u00ED"
    readonly property string newMaterial: "\u00EE"
    readonly property string nextFile_large: "\u00EF"
    readonly property string normalBar_small: "\u00F0"
    readonly property string openLink: "\u00F1"
    readonly property string openMaterialBrowser: "\u00F2"
    readonly property string orientation: "\u00F3"
    readonly property string orthCam_medium: "\u00F4"
    readonly property string orthCam_small: "\u00F5"
    readonly property string paddingEdge: "\u00F6"
    readonly property string paddingFrame: "\u00F7"
    readonly property string particleAnimation_medium: "\u00F8"
    readonly property string pasteStyle: "\u00F9"
    readonly property string paste_small: "\u00FA"
    readonly property string pause: "\u00FB"
    readonly property string pause_medium: "\u00FC"
    readonly property string perspectiveCam_medium: "\u00FD"
    readonly property string perspectiveCam_small: "\u00FE"
    readonly property string pin: "\u00FF"
    readonly property string plane_medium: "\u0100"
    readonly property string plane_small: "\u0101"
    readonly property string play: "\u0102"
    readonly property string playFill_medium: "\u0103"
    readonly property string playOutline_medium: "\u0104"
    readonly property string plus: "\u0105"
    readonly property string pointLight_small: "\u0106"
    readonly property string positioners_small: "\u0107"
    readonly property string previewEnv_medium: "\u0108"
    readonly property string previousFile_large: "\u0109"
    readonly property string promote: "\u010A"
    readonly property string properties_medium: "\u010B"
    readonly property string readOnly: "\u010C"
    readonly property string recent_medium: "\u010D"
    readonly property string recordFill_medium: "\u010E"
    readonly property string recordOutline_medium: "\u010F"
    readonly property string redo: "\u0110"
    readonly property string reload_medium: "\u0111"
    readonly property string remove_medium: "\u0112"
    readonly property string remove_small: "\u0113"
    readonly property string rename_small: "\u0114"
    readonly property string replace_small: "\u0115"
    readonly property string resetView_small: "\u0116"
    readonly property string restartParticles_medium: "\u0117"
    readonly property string reverseOrder_medium: "\u0118"
    readonly property string roatate_medium: "\u0119"
    readonly property string rotationFill: "\u011A"
    readonly property string rotationOutline: "\u011B"
    readonly property string runProjFill_large: "\u011C"
    readonly property string runProjOutline_large: "\u011D"
    readonly property string s_anchors: "\u011E"
    readonly property string s_annotations: "\u011F"
    readonly property string s_arrange: "\u0120"
    readonly property string s_boundingBox: "\u0121"
    readonly property string s_component: "\u0122"
    readonly property string s_connections: "\u0123"
    readonly property string s_edit: "\u0124"
    readonly property string s_enterComponent: "\u0125"
    readonly property string s_eventList: "\u0126"
    readonly property string s_group: "\u0127"
    readonly property string s_layouts: "\u0128"
    readonly property string s_merging: "\u0129"
    readonly property string s_mouseArea: "\u012A"
    readonly property string s_positioners: "\u012B"
    readonly property string s_selection: "\u012C"
    readonly property string s_snapping: "\u012D"
    readonly property string s_timeline: "\u012E"
    readonly property string s_visibility: "\u012F"
    readonly property string saveAs_medium: "\u0130"
    readonly property string saveLogs_medium: "\u0131"
    readonly property string save_medium: "\u0132"
    readonly property string scale_medium: "\u0133"
    readonly property string search: "\u0134"
    readonly property string search_small: "\u0135"
    readonly property string sectionToggle: "\u0136"
    readonly property string selectFill_medium: "\u0137"
    readonly property string selectOutline_medium: "\u0138"
    readonly property string selectParent_small: "\u0139"
    readonly property string selection_small: "\u013A"
    readonly property string settings_medium: "\u013B"
    readonly property string signal_small: "\u013C"
    readonly property string snapping_conf_medium: "\u013D"
    readonly property string snapping_medium: "\u013E"
    readonly property string snapping_small: "\u013F"
    readonly property string sortascending_medium: "\u0140"
    readonly property string sortdescending_medium: "\u0141"
    readonly property string sphere_medium: "\u0142"
    readonly property string sphere_small: "\u0143"
    readonly property string splitColumns: "\u0144"
    readonly property string splitRows: "\u0145"
    readonly property string splitScreen_medium: "\u0146"
    readonly property string spotLight_small: "\u0147"
    readonly property string stackedContainer_small: "\u0148"
    readonly property string startNode: "\u0149"
    readonly property string step_medium: "\u014A"
    readonly property string stop_medium: "\u014B"
    readonly property string tableView_medium: "\u014C"
    readonly property string testIcon: "\u014D"
    readonly property string textAlignBottom: "\u014E"
    readonly property string textAlignCenter: "\u014F"
    readonly property string textAlignJustified: "\u0150"
    readonly property string textAlignLeft: "\u0151"
    readonly property string textAlignMiddle: "\u0152"
    readonly property string textAlignRight: "\u0153"
    readonly property string textAlignTop: "\u0154"
    readonly property string textBulletList: "\u0155"
    readonly property string textFullJustification: "\u0156"
    readonly property string textNumberedList: "\u0157"
    readonly property string textures_medium: "\u0158"
    readonly property string tickIcon: "\u0159"
    readonly property string tickMark_small: "\u015A"
    readonly property string timeline_small: "\u015B"
    readonly property string toEndFrame_medium: "\u015C"
    readonly property string toNextFrame_medium: "\u015D"
    readonly property string toPrevFrame_medium: "\u015E"
    readonly property string toStartFrame_medium: "\u015F"
    readonly property string topToolbar_annotations: "\u0160"
    readonly property string topToolbar_closeFile: "\u0161"
    readonly property string topToolbar_designMode: "\u0162"
    readonly property string topToolbar_enterComponent: "\u0163"
    readonly property string topToolbar_home: "\u0164"
    readonly property string topToolbar_makeComponent: "\u0165"
    readonly property string topToolbar_navFile: "\u0166"
    readonly property string topToolbar_runProject: "\u0167"
    readonly property string translationCreateFiles: "\u0168"
    readonly property string translationCreateReport: "\u0169"
    readonly property string translationExport: "\u016A"
    readonly property string translationImport: "\u016B"
    readonly property string translationSelectLanguages: "\u016C"
    readonly property string translationTest: "\u016D"
    readonly property string transparent: "\u016E"
    readonly property string triState: "\u016F"
    readonly property string triangleArcA: "\u0170"
    readonly property string triangleArcB: "\u0171"
    readonly property string triangleCornerA: "\u0172"
    readonly property string triangleCornerB: "\u0173"
    readonly property string unLinked: "\u0174"
    readonly property string undo: "\u0175"
    readonly property string unify_medium: "\u0176"
    readonly property string unpin: "\u0177"
    readonly property string upDownIcon: "\u0178"
    readonly property string upDownSquare2: "\u0179"
    readonly property string updateAvailable_medium: "\u017A"
    readonly property string updateContent_medium: "\u017B"
    readonly property string visibilityOff: "\u017C"
    readonly property string visibilityOn: "\u017D"
    readonly property string visible_medium: "\u017E"
    readonly property string visible_small: "\u017F"
    readonly property string warning_medium: "\u0180"
    readonly property string wildcard: "\u0181"
    readonly property string wizardsAutomotive: "\u0182"
    readonly property string wizardsDesktop: "\u0183"
    readonly property string wizardsGeneric: "\u0184"
    readonly property string wizardsMcuEmpty: "\u0185"
    readonly property string wizardsMcuGraph: "\u0186"
    readonly property string wizardsMobile: "\u0187"
    readonly property string wizardsUnknown: "\u0188"
    readonly property string zoomAll: "\u0189"
    readonly property string zoomIn: "\u018A"
    readonly property string zoomIn_medium: "\u018B"
    readonly property string zoomOut: "\u018C"
    readonly property string zoomOut_medium: "\u018D"
    readonly property string zoomSelection: "\u018E"

    readonly property font iconFont: Qt.font({
                                                 "family": controlIcons.name,
                                                 "pixelSize": 12
                                             })

    readonly property font font: Qt.font({
                                             "family": "Arial",
                                             "pointSize": Qt.application.font.pixelSize
                                         })

    readonly property font largeFont: Qt.font({
                                                  "family": "Arial",
                                                  "pointSize": Qt.application.font.pixelSize * 1.6
                                              })
}
