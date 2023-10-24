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
    readonly property string addRowAfter: "\u0026"
    readonly property string addRowBefore: "\u0027"
    readonly property string addTable: "\u0028"
    readonly property string add_medium: "\u0029"
    readonly property string add_small: "\u002A"
    readonly property string addcolumnleft_medium: "\u002B"
    readonly property string addcolumnright_medium: "\u002C"
    readonly property string addrowabove_medium: "\u002D"
    readonly property string addrowbelow_medium: "\u002E"
    readonly property string adsClose: "\u002F"
    readonly property string adsDetach: "\u0030"
    readonly property string adsDropDown: "\u0031"
    readonly property string alias: "\u0032"
    readonly property string aliasAnimated: "\u0033"
    readonly property string alignBottom: "\u0034"
    readonly property string alignCenterHorizontal: "\u0035"
    readonly property string alignCenterVertical: "\u0036"
    readonly property string alignLeft: "\u0037"
    readonly property string alignRight: "\u0038"
    readonly property string alignTo: "\u0039"
    readonly property string alignToCam_medium: "\u003A"
    readonly property string alignToCamera_small: "\u003B"
    readonly property string alignToObject_small: "\u003C"
    readonly property string alignToView_medium: "\u003D"
    readonly property string alignTop: "\u003E"
    readonly property string anchorBaseline: "\u003F"
    readonly property string anchorBottom: "\u0040"
    readonly property string anchorFill: "\u0041"
    readonly property string anchorLeft: "\u0042"
    readonly property string anchorRight: "\u0043"
    readonly property string anchorTop: "\u0044"
    readonly property string anchors_small: "\u0045"
    readonly property string animatedProperty: "\u0046"
    readonly property string annotationBubble: "\u0047"
    readonly property string annotationDecal: "\u0048"
    readonly property string annotations_large: "\u0049"
    readonly property string annotations_small: "\u004A"
    readonly property string applyMaterialToSelected: "\u004B"
    readonly property string apply_medium: "\u004C"
    readonly property string apply_small: "\u004D"
    readonly property string arrange_small: "\u004E"
    readonly property string arrow_small: "\u004F"
    readonly property string assign: "\u0050"
    readonly property string attach_medium: "\u0051"
    readonly property string back_medium: "\u0052"
    readonly property string backspace_small: "\u0053"
    readonly property string bevelAll: "\u0054"
    readonly property string bevelCorner: "\u0055"
    readonly property string bezier_medium: "\u0056"
    readonly property string binding_medium: "\u0057"
    readonly property string bounds_small: "\u0058"
    readonly property string branch_medium: "\u0059"
    readonly property string camera_small: "\u005A"
    readonly property string centerHorizontal: "\u005B"
    readonly property string centerVertical: "\u005C"
    readonly property string cleanLogs_medium: "\u005D"
    readonly property string closeCross: "\u005E"
    readonly property string closeFile_large: "\u005F"
    readonly property string closeLink: "\u0060"
    readonly property string close_small: "\u0061"
    readonly property string code: "\u0062"
    readonly property string codeEditor_medium: "\u0063"
    readonly property string codeview_medium: "\u0064"
    readonly property string colorPopupClose: "\u0065"
    readonly property string colorSelection_medium: "\u0066"
    readonly property string columnsAndRows: "\u0067"
    readonly property string cone_medium: "\u0068"
    readonly property string cone_small: "\u0069"
    readonly property string connection_small: "\u006A"
    readonly property string connections_medium: "\u006B"
    readonly property string copyLink: "\u006C"
    readonly property string copyStyle: "\u006D"
    readonly property string copy_small: "\u006E"
    readonly property string cornerA: "\u006F"
    readonly property string cornerB: "\u0070"
    readonly property string cornersAll: "\u0071"
    readonly property string createComponent_large: "\u0072"
    readonly property string createComponent_small: "\u0073"
    readonly property string create_medium: "\u0074"
    readonly property string create_small: "\u0075"
    readonly property string cube_medium: "\u0076"
    readonly property string cube_small: "\u0077"
    readonly property string curveDesigner: "\u0078"
    readonly property string curveDesigner_medium: "\u0079"
    readonly property string curveEditor: "\u007A"
    readonly property string customMaterialEditor: "\u007B"
    readonly property string cylinder_medium: "\u007C"
    readonly property string cylinder_small: "\u007D"
    readonly property string decisionNode: "\u007E"
    readonly property string deleteColumn: "\u007F"
    readonly property string deleteMaterial: "\u0080"
    readonly property string deleteRow: "\u0081"
    readonly property string deleteTable: "\u0082"
    readonly property string delete_medium: "\u0083"
    readonly property string delete_small: "\u0084"
    readonly property string deletecolumn_medium: "\u0085"
    readonly property string deleterow_medium: "\u0086"
    readonly property string designMode_large: "\u0087"
    readonly property string detach: "\u0088"
    readonly property string directionalLight_small: "\u0089"
    readonly property string distributeBottom: "\u008A"
    readonly property string distributeCenterHorizontal: "\u008B"
    readonly property string distributeCenterVertical: "\u008C"
    readonly property string distributeLeft: "\u008D"
    readonly property string distributeOriginBottomRight: "\u008E"
    readonly property string distributeOriginCenter: "\u008F"
    readonly property string distributeOriginNone: "\u0090"
    readonly property string distributeOriginTopLeft: "\u0091"
    readonly property string distributeRight: "\u0092"
    readonly property string distributeSpacingHorizontal: "\u0093"
    readonly property string distributeSpacingVertical: "\u0094"
    readonly property string distributeTop: "\u0095"
    readonly property string download: "\u0096"
    readonly property string downloadUnavailable: "\u0097"
    readonly property string downloadUpdate: "\u0098"
    readonly property string downloadcsv_large: "\u0099"
    readonly property string downloadcsv_medium: "\u009A"
    readonly property string downloaded: "\u009B"
    readonly property string downloadjson_large: "\u009D"
    readonly property string downloadjson_medium: "\u009E"
    readonly property string dragmarks: "\u009F"
    readonly property string duplicate_small: "\u00A0"
    readonly property string edit: "\u00A1"
    readonly property string editComponent_large: "\u00A2"
    readonly property string editComponent_small: "\u00A3"
    readonly property string editLightOff_medium: "\u00A4"
    readonly property string editLightOn_medium: "\u00A5"
    readonly property string edit_medium: "\u00A6"
    readonly property string edit_small: "\u00A7"
    readonly property string effects: "\u00A8"
    readonly property string events_small: "\u00A9"
    readonly property string export_medium: "\u00AA"
    readonly property string eyeDropper: "\u00AB"
    readonly property string favorite: "\u00AC"
    readonly property string fitAll_medium: "\u00AE"
    readonly property string fitSelected_small: "\u00AF"
    readonly property string fitSelection_medium: "\u00B0"
    readonly property string fitToView_medium: "\u00B1"
    readonly property string flowAction: "\u00B2"
    readonly property string flowTransition: "\u00B3"
    readonly property string fontStyleBold: "\u00B4"
    readonly property string fontStyleItalic: "\u00B5"
    readonly property string fontStyleStrikethrough: "\u00B6"
    readonly property string fontStyleUnderline: "\u00B7"
    readonly property string forward_medium: "\u00B8"
    readonly property string globalOrient_medium: "\u00B9"
    readonly property string gradient: "\u00BA"
    readonly property string gridView: "\u00BB"
    readonly property string grid_medium: "\u00BC"
    readonly property string group_small: "\u00BD"
    readonly property string help: "\u00BE"
    readonly property string home_large: "\u00BF"
    readonly property string idAliasOff: "\u00C0"
    readonly property string idAliasOn: "\u00C1"
    readonly property string import_medium: "\u00C2"
    readonly property string imported: "\u00C3"
    readonly property string importedModels_small: "\u00C4"
    readonly property string infinity: "\u00C5"
    readonly property string invisible_medium: "\u00C6"
    readonly property string invisible_small: "\u00C7"
    readonly property string jumpToCode_medium: "\u00C8"
    readonly property string jumpToCode_small: "\u00C9"
    readonly property string keyframe: "\u00CA"
    readonly property string languageList_medium: "\u00CB"
    readonly property string layouts_small: "\u00CC"
    readonly property string lights_small: "\u00CD"
    readonly property string linear_medium: "\u00CE"
    readonly property string linkTriangle: "\u00CF"
    readonly property string linked: "\u00D0"
    readonly property string listView: "\u00D1"
    readonly property string list_medium: "\u00D2"
    readonly property string localOrient_medium: "\u00D3"
    readonly property string lockOff: "\u00D4"
    readonly property string lockOn: "\u00D5"
    readonly property string loopPlayback_medium: "\u00D6"
    readonly property string materialBrowser_medium: "\u00D7"
    readonly property string materialPreviewEnvironment: "\u00D8"
    readonly property string materialPreviewModel: "\u00D9"
    readonly property string material_medium: "\u00DA"
    readonly property string maxBar_small: "\u00DB"
    readonly property string mergeCells: "\u00DC"
    readonly property string merge_small: "\u00DD"
    readonly property string minus: "\u00DE"
    readonly property string mirror: "\u00DF"
    readonly property string more_medium: "\u00E0"
    readonly property string mouseArea_small: "\u00E1"
    readonly property string moveDown_medium: "\u00E2"
    readonly property string moveInwards_medium: "\u00E3"
    readonly property string moveUp_medium: "\u00E4"
    readonly property string moveUpwards_medium: "\u00E5"
    readonly property string move_medium: "\u00E6"
    readonly property string newMaterial: "\u00E7"
    readonly property string nextFile_large: "\u00E8"
    readonly property string normalBar_small: "\u00E9"
    readonly property string openLink: "\u00EA"
    readonly property string openMaterialBrowser: "\u00EB"
    readonly property string orientation: "\u00EC"
    readonly property string orthCam_medium: "\u00ED"
    readonly property string orthCam_small: "\u00EE"
    readonly property string paddingEdge: "\u00EF"
    readonly property string paddingFrame: "\u00F0"
    readonly property string particleAnimation_medium: "\u00F1"
    readonly property string pasteStyle: "\u00F2"
    readonly property string paste_small: "\u00F3"
    readonly property string pause: "\u00F4"
    readonly property string perspectiveCam_medium: "\u00F5"
    readonly property string perspectiveCam_small: "\u00F6"
    readonly property string pin: "\u00F7"
    readonly property string plane_medium: "\u00F8"
    readonly property string plane_small: "\u00F9"
    readonly property string play: "\u00FA"
    readonly property string playFill_medium: "\u00FB"
    readonly property string playOutline_medium: "\u00FC"
    readonly property string plus: "\u00FD"
    readonly property string pointLight_small: "\u00FE"
    readonly property string positioners_small: "\u00FF"
    readonly property string previewEnv_medium: "\u0100"
    readonly property string previousFile_large: "\u0101"
    readonly property string promote: "\u0102"
    readonly property string properties_medium: "\u0103"
    readonly property string readOnly: "\u0104"
    readonly property string recordFill_medium: "\u0105"
    readonly property string recordOutline_medium: "\u0106"
    readonly property string redo: "\u0107"
    readonly property string reload_medium: "\u0108"
    readonly property string remove_medium: "\u0109"
    readonly property string remove_small: "\u010A"
    readonly property string rename_small: "\u010B"
    readonly property string replace_small: "\u010C"
    readonly property string resetView_small: "\u010D"
    readonly property string restartParticles_medium: "\u010E"
    readonly property string reverseOrder_medium: "\u010F"
    readonly property string roatate_medium: "\u0110"
    readonly property string rotationFill: "\u0111"
    readonly property string rotationOutline: "\u0112"
    readonly property string runProjFill_large: "\u0113"
    readonly property string runProjOutline_large: "\u0114"
    readonly property string s_anchors: "\u0115"
    readonly property string s_annotations: "\u0116"
    readonly property string s_arrange: "\u0117"
    readonly property string s_boundingBox: "\u0118"
    readonly property string s_component: "\u0119"
    readonly property string s_connections: "\u011A"
    readonly property string s_edit: "\u011B"
    readonly property string s_enterComponent: "\u011C"
    readonly property string s_eventList: "\u011D"
    readonly property string s_group: "\u011E"
    readonly property string s_layouts: "\u011F"
    readonly property string s_merging: "\u0120"
    readonly property string s_mouseArea: "\u0121"
    readonly property string s_positioners: "\u0122"
    readonly property string s_selection: "\u0123"
    readonly property string s_snapping: "\u0124"
    readonly property string s_timeline: "\u0125"
    readonly property string s_visibility: "\u0126"
    readonly property string saveLogs_medium: "\u0127"
    readonly property string scale_medium: "\u0128"
    readonly property string search: "\u0129"
    readonly property string search_small: "\u012A"
    readonly property string sectionToggle: "\u012B"
    readonly property string selectFill_medium: "\u012C"
    readonly property string selectOutline_medium: "\u012D"
    readonly property string selectParent_small: "\u012E"
    readonly property string selection_small: "\u012F"
    readonly property string settings_medium: "\u0130"
    readonly property string signal_small: "\u0131"
    readonly property string snapping_conf_medium: "\u0132"
    readonly property string snapping_medium: "\u0133"
    readonly property string snapping_small: "\u0134"
    readonly property string sortascending_medium: "\u0135"
    readonly property string sortdescending_medium: "\u0136"
    readonly property string sphere_medium: "\u0137"
    readonly property string sphere_small: "\u0138"
    readonly property string splitColumns: "\u0139"
    readonly property string splitRows: "\u013A"
    readonly property string spotLight_small: "\u013B"
    readonly property string stackedContainer_small: "\u013C"
    readonly property string startNode: "\u013D"
    readonly property string step_medium: "\u013E"
    readonly property string stop_medium: "\u013F"
    readonly property string testIcon: "\u0140"
    readonly property string textAlignBottom: "\u0141"
    readonly property string textAlignCenter: "\u0142"
    readonly property string textAlignJustified: "\u0143"
    readonly property string textAlignLeft: "\u0144"
    readonly property string textAlignMiddle: "\u0145"
    readonly property string textAlignRight: "\u0146"
    readonly property string textAlignTop: "\u0147"
    readonly property string textBulletList: "\u0148"
    readonly property string textFullJustification: "\u0149"
    readonly property string textNumberedList: "\u014A"
    readonly property string textures_medium: "\u014B"
    readonly property string tickIcon: "\u014C"
    readonly property string tickMark_small: "\u014D"
    readonly property string timeline_small: "\u014E"
    readonly property string toEndFrame_medium: "\u014F"
    readonly property string toNextFrame_medium: "\u0150"
    readonly property string toPrevFrame_medium: "\u0151"
    readonly property string toStartFrame_medium: "\u0152"
    readonly property string topToolbar_annotations: "\u0153"
    readonly property string topToolbar_closeFile: "\u0154"
    readonly property string topToolbar_designMode: "\u0155"
    readonly property string topToolbar_enterComponent: "\u0156"
    readonly property string topToolbar_home: "\u0157"
    readonly property string topToolbar_makeComponent: "\u0158"
    readonly property string topToolbar_navFile: "\u0159"
    readonly property string topToolbar_runProject: "\u015A"
    readonly property string translationCreateFiles: "\u015B"
    readonly property string translationCreateReport: "\u015C"
    readonly property string translationExport: "\u015D"
    readonly property string translationImport: "\u015E"
    readonly property string translationSelectLanguages: "\u015F"
    readonly property string translationTest: "\u0160"
    readonly property string transparent: "\u0161"
    readonly property string triState: "\u0162"
    readonly property string triangleArcA: "\u0163"
    readonly property string triangleArcB: "\u0164"
    readonly property string triangleCornerA: "\u0165"
    readonly property string triangleCornerB: "\u0166"
    readonly property string unLinked: "\u0167"
    readonly property string undo: "\u0168"
    readonly property string unify_medium: "\u0169"
    readonly property string unpin: "\u016A"
    readonly property string upDownIcon: "\u016B"
    readonly property string upDownSquare2: "\u016C"
    readonly property string updateAvailable_medium: "\u016D"
    readonly property string updateContent_medium: "\u016E"
    readonly property string uploadcsv_large: "\u016F"
    readonly property string uploadcsv_medium: "\u0170"
    readonly property string uploadjson_large: "\u0171"
    readonly property string uploadjson_medium: "\u0172"
    readonly property string visibilityOff: "\u0173"
    readonly property string visibilityOn: "\u0174"
    readonly property string visible_medium: "\u0175"
    readonly property string visible_small: "\u0176"
    readonly property string wildcard: "\u0177"
    readonly property string wizardsAutomotive: "\u0178"
    readonly property string wizardsDesktop: "\u0179"
    readonly property string wizardsGeneric: "\u017A"
    readonly property string wizardsMcuEmpty: "\u017B"
    readonly property string wizardsMcuGraph: "\u017C"
    readonly property string wizardsMobile: "\u017D"
    readonly property string wizardsUnknown: "\u017E"
    readonly property string zoomAll: "\u017F"
    readonly property string zoomIn: "\u0180"
    readonly property string zoomIn_medium: "\u0181"
    readonly property string zoomOut: "\u0182"
    readonly property string zoomOut_medium: "\u0183"
    readonly property string zoomSelection: "\u0184"

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
