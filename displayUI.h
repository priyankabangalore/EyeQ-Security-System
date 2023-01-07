#ifndef GROUP3_DISPLAYUI_H
    #define GROUP3_DISPLAYUI_H

    // Include statements
    #include <Wt/WApplication.h>
    #include <Wt/WBreak.h>
    #include <Wt/WContainerWidget.h>
    #include <Wt/WLineEdit.h>
    #include <Wt/WPushButton.h>
    #include <Wt/WText.h>
    #include <Wt/WLink.h>
    #include <Wt/WMediaPlayer.h>
    #include <Wt/WImage.h>
    #include <Wt/WVideo.h>
    #include <Wt/WFont.h>
    #include <Wt/WCssDecorationStyle.h>
    #include <Wt/WHBoxLayout.h>
    #include <Wt/WTable.h>
    #include <Wt/WTableCell.h>
    #include <Wt/WCheckBox.h>
    #include <Wt/WTemplate.h>
    #include <stdio.h>
    #include <Wt/WStringListModel.h>
    #include <Wt/WAny.h>
    #include <Wt/WComboBox.h>
    #include <iostream>
    #include <string.h>
    #include <sstream>
    #include <fstream>
    #include <cstring>
    #include <vector>
    #include <ctime>
    #include <string>
    #include <locale>
    #include <codecvt>

/*!
 \class DisplayUI
 @brief The front-end for the security system that performs all necessary functions.
 The DisplayUI class displays the UI to HTML, with functionalities of displaying live security footage obtained from a camera, 
 displaying and updating the user log database, displaying a user's QR code and displaying weather obtained from the weather sensor.
 @author(s) Disha Puri and Priyanka Bangalore
*/
class DisplayUI : public Wt::WApplication
{
public:
    DisplayUI(const Wt::WEnvironment& env);
};

#endif //GROUP3_DISPLAYUI_H
