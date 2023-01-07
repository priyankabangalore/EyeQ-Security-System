#include "displayUI.h"

using namespace Wt;
using namespace std;

/*!
 \fn DisplayUI
 @brief The constructor for the front-end of the security system.
 The DisplayUI constructor displays the UI to HTML, with functionalities of displaying live security footage obtained from a camera, 
 displaying and updating the user log database, displaying a user's QR code and displaying weather obtained from the weather sensor.
 @param WEnvironment Wt environment for running
 @author(s) Disha Puri and Priyanka Bangalore
*/
DisplayUI::DisplayUI(const WEnvironment& env)
        : WApplication(env)
{
    setTitle("EyeQ Security");

    useStyleSheet("style.css");

    // A container that covers the whole page
    auto pageContainer = root()->addWidget(make_unique<WContainerWidget>());
    pageContainer->setStyleClass("pageContainer");

    // The page's title
    auto titleContainer = pageContainer->addNew<WContainerWidget>();
    auto title = titleContainer->addNew<WText>();
    title->setText("EyeQ Security");
    titleContainer->setStyleClass("titleContainer");

    // A button that links to our Raspberry Pi's video footage
    auto videoLinkContainer = pageContainer->addNew<WContainerWidget>();
    auto viewButton = videoLinkContainer->addNew<WPushButton>("View Footage");
    videoLinkContainer->addStyleClass("videoLinkContainer");
    viewButton->setLink("http://192.168.43.139:8081/");
    viewButton->addStyleClass("btn-info");

    // A smaller container for stylization
    auto container = pageContainer->addNew<WContainerWidget>();
    container->setStyleClass("container");

    // The container and title for the user status list
    auto userListContainer = container->addNew<WContainerWidget>();
    auto userListTitle = userListContainer->addNew<WContainerWidget>();
    auto title1 = userListTitle->addNew<WText>();
    title1->setText("<h2> Update Status </h2>");
    userListTitle->addStyleClass("userListTitle");

    // The drop down menu for selecting a user
    auto cb = userListContainer->addNew<WComboBox>();
    cb->addStyleClass("selectMenu");
    cb->addItem("dpuri9");
    cb->addItem("pbangalo");
    cb->addItem("asheik55");
    cb->addItem("iginson");
    cb->addItem("wshi84");
    cb->setNoSelectionEnabled(true);
    auto out = userListContainer->addNew<WText>();

    // Displaying the user's QR code
    WImage *image = container->addNew<WImage>(WLink("dpuri9.jpg"));
    image->setStyleClass("image");

    // Actions when the button is pressed
    cb->changed().connect([=] {

        // Converts the selected user's name from the drop-down menu into a string variable
        WString selectedUser = cb->currentText();
        int row = cb->currentIndex();
        using convert_type = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_type, wchar_t> converter;
        std::string converted_str = converter.to_bytes(selectedUser);

        // Creates a link for the user's associated QR code to be displayed onto screen
        converted_str += ".jpg";
        image->setImageLink(WLink(converted_str));
        image->setAlternateText(converted_str);
        image->setStyleClass("image");

    });

    // A user log to display the activity of all users
    auto userLogContainer = container->addNew<WContainerWidget>();
    userLogContainer->setStyleClass("logContainer");
    auto logTitle = userLogContainer->addNew<WText>();
    logTitle->setText("<h2> User Log </h2>");
    logTitle->setStyleClass("logTitle");

    auto tableContainer = userLogContainer->addNew<WContainerWidget>();
    tableContainer->setStyleClass("tableContainer");
    auto logTable = tableContainer->addNew<WTable>();
    logTable->setHeaderCount(1);
    logTable->setWidth(WLength("100%"));

    // Set table headers
    logTable->elementAt(0, 0)->addNew<WText>("<div class='headElem'> <b> User </b> </div>");
    logTable->elementAt(0, 1)->addNew<WText>("<div class='headElem'> Status </div>");
    logTable->elementAt(0, 2)->addNew<WText>("<div class='headElem'> Timestamp </div>");

    // Fills table with data from userlog.txt
    string username;
    vector<string> userInfo;
    fstream newfile;

    // Opens a file to perform write operation using file object
    newfile.open("userlog.txt", ios::in);
    if (newfile.is_open())
    {
        // Checks whether the file is open
        if (newfile.is_open()) {
            string tp;
            // Reads data from file object and put it into a vector
            while (getline(newfile, tp, ',')) {
                userInfo.push_back(tp);
            }
            newfile.close();
        }
    }

    // Add usernames to table
    int count = 1;
    for (int x=0; x < userInfo.size(); x+=3) {
        logTable->elementAt(count, 0)->addNew<WText>("<div class = 'elem'> " + userInfo[x] + " </div>");
        count++;
    }

    // Add statuses to table
    count = 1;
    for (int x=1; x < userInfo.size(); x+=3) {
        logTable->elementAt(count, 1)->addNew<WText>("<div class = 'elem'> " + userInfo[x] + " </div>");
        count++;
    }

    // Add timestamp to table
    count = 1;
    for (int x=2; x < userInfo.size(); x+=3) {
        logTable->elementAt(count, 2)->addNew<WText>("<div class = 'elem'> " + userInfo[x] + " </div>");
        count++;
    }

    // Stylize the table
    logTable->addStyleClass("logTable");

    // Creates a button to allow the selected user to check out of the log
    auto checkButtons = userListContainer->addNew<WContainerWidget>();
    userListContainer->addStyleClass("userListContainer");
    auto result1 = checkButtons->addNew<WTemplate>();
    result1->setTemplateText("<div> ${checkOut} </div>");
    auto checkOut = result1->bindWidget("checkOut", make_unique<WPushButton>("Check Out"));
    checkOut->setStyleClass("btn-info");
    checkOut->clicked().connect( [=] {

        // Current date/time based on current system
        time_t now = time(0);
        // Convert now to string form
        char* dt = ctime(&now);

        // Updates the userlog and adds the desired action
        ofstream myfile;
        myfile.open ("userlog.txt", ofstream::app);
        myfile << cb->currentText() << ", Left, " << dt << ",";
        myfile.close();

        // Clears the current log and repopulates the headers
        logTable->clear();
        logTable->elementAt(0, 0)->addNew<WText>("<div class='headElem'> <b> User </b> </div>");
        logTable->elementAt(0, 1)->addNew<WText>("<div class='headElem'> Status </div>");
        logTable->elementAt(0, 2)->addNew<WText>("<div class='headElem'> Timestamp </div>");

        // Opens a file to perform write operation using file object
        fstream file;
        vector<string> userInfoVec;
        file.open("userlog.txt", ios::in);
        if (file.is_open()) {
            // Checks whether the file is open
            if (file.is_open()) {
                string tp;
                // Reads data from file object and put it into string
                while (getline(file, tp, ',')) {
                    userInfoVec.push_back(tp);
                }
                file.close();
            }
        }

        // Add usernames to table
        int count = 1;
        for (int x=0; x < userInfoVec.size(); x+=3) {
            logTable->elementAt(count, 0)->addNew<WText>("<div class = 'elem'> " + userInfoVec[x] + " </div>");
            count++;
        }

        // Add statuses to table
        count = 1;
        for (int x=1; x < userInfoVec.size(); x+=3) {
            logTable->elementAt(count, 1)->addNew<WText>("<div class = 'elem'> " + userInfoVec[x] + " </div>");
            count++;
        }

        // Add timestamp to table
        count = 1;
        for (int x=2; x < userInfoVec.size(); x+=3) {
            logTable->elementAt(count, 2)->addNew<WText>("<div class = 'elem'> " + userInfoVec[x] + " </div>");
            count++;
        }

    });

    // Creates a button to allow the selected user to check out of the log
    auto result = checkButtons->addNew<WTemplate>();
    result->setTemplateText("<div> ${checkIn} </div>");
    auto checkIn = result->bindWidget("checkIn", make_unique<WPushButton>("Check In"));
    checkIn->setStyleClass("btn-info");
    checkIn->clicked().connect( [=] {

        // Current date/time based on current system
        time_t now = time(0);
        // Convert now to string form
        char* dt = ctime(&now);

        // Updates the userlog and adds the desired action
        ofstream myfile;
        myfile.open ("userlog.txt", ofstream::app);
        myfile << cb->currentText() << ", Entered, " << dt << ",";
        myfile.close();

        // Clears the current log and repopulates the headers
        logTable->clear();
        logTable->elementAt(0, 0)->addNew<WText>("<div class='headElem'> <b> User </b> </div>");
        logTable->elementAt(0, 1)->addNew<WText>("<div class='headElem'> Status </div>");
        logTable->elementAt(0, 2)->addNew<WText>("<div class='headElem'> Timestamp </div>");

        // Opens a file to perform write operation using file object
        fstream file;
        vector<string> userInfoVec;
        file.open("userlog.txt", ios::in);
        if (file.is_open()) {
            // Checks whether the file is open
            if (file.is_open()) {
                string tp;
                // Reads data from file object and put it into string
                while (getline(file, tp, ',')) {
                    userInfoVec.push_back(tp);
                }
                file.close();
            }
        }

        // Add usernames to table
        int count = 1;
        for (int x=0; x < userInfoVec.size(); x+=3) {
            logTable->elementAt(count, 0)->addNew<WText>("<div class = 'elem'> " + userInfoVec[x] + " </div>");
            count++;
        }

        // Add statuses to table
        count = 1;
        for (int x=1; x < userInfoVec.size(); x+=3) {
            logTable->elementAt(count, 1)->addNew<WText>("<div class = 'elem'> " + userInfoVec[x] + " </div>");
            count++;
        }

        // Add timestamp to table
        count = 1;
        for (int x=2; x < userInfoVec.size(); x+=3) {
            logTable->elementAt(count, 2)->addNew<WText>("<div class = 'elem'> " + userInfoVec[x] + " </div>");
            count++;
        }
    });

    // Creates a button that clears the log of all data
    auto result3 = checkButtons->addNew<WTemplate>();
    result3->setTemplateText("<div> ${clearLog} </div>");
    auto clearLog = result3->bindWidget("clearLog", make_unique<WPushButton>("Clear Log"));
    clearLog->setStyleClass("btn-info");
    clearLog->clicked().connect( [=] {
        ofstream ofs;
        ofs.open("userlog.txt", std::ofstream::out | std::ofstream::trunc);
        ofs.close();

        // Clears the current log and repopulates the headers
        logTable->clear();
        logTable->elementAt(0, 0)->addNew<WText>("<div class='headElem'> <b> User </b> </div>");
        logTable->elementAt(0, 1)->addNew<WText>("<div class='headElem'> Status </div>");
        logTable->elementAt(0, 2)->addNew<WText>("<div class='headElem'> Timestamp </div>");

        // Opens a file to perform write operation using file object
        fstream file;
        vector<string> userInfoVec;
        file.open("userlog.txt", ios::in);
        if (file.is_open()) {
            // Checks whether the file is open
            if (file.is_open()) {
                string tp;
                // Reads data from file object and put it into string
                while (getline(file, tp, ',')) {
                    userInfoVec.push_back(tp);
                }
                file.close();
            }
        }
    });

    // Weather display
    auto weatherContainer = pageContainer->addNew<WContainerWidget>();
    weatherContainer->setStyleClass("weatherContainer");
    auto weatherTitle = weatherContainer->addNew<WText>();
    weatherTitle->setText("<h2> Weather </h2>");
    weatherTitle->setStyleClass("weatherTitle");

    // Reads weather information from a text file that has been updated by the Raspberry Pi
    fstream weatherFile;
    vector<string> weatherInfoDoc;

    system("sshpass -p password scp -o StrictHostKeyChecking=no pi@192.168.43.139:/home/pi/group3/BME280-Environmental-Sensor-Demo-Code/weathers.txt ~/weathers.txt");
    weatherFile.open("/Users/dishapuri/weathers.txt", ios::in);
    if (weatherFile.is_open()) {
        // Checks whether the file is open
            string tx;
            // Reads data from file object and put it into string
            while (getline(weatherFile, tx, ',')) {
                weatherInfoDoc.push_back(tx);
            }
            weatherFile.close();
    }

    // Weather log
    auto weatherLogContainer = weatherContainer->addNew<WContainerWidget>();
    weatherLogContainer->setStyleClass("weatherContainer");
    auto weatherLogTitle = weatherLogContainer->addNew<WText>();

    auto weatherLog = weatherLogContainer->addNew<WTable>();
    weatherLog->setHeaderCount(1);
    weatherLog->setWidth(WLength("100%"));

    // Set weather table headers
    weatherLog->elementAt(0, 0)->addNew<WText>("<div class='WheadElem'> <b> Temperature </b> </div>");
    weatherLog->elementAt(0, 1)->addNew<WText>("<div class='WheadElem'> Pressure </div>");
    weatherLog->elementAt(0, 2)->addNew<WText>("<div class='WheadElem'> Humidity </div>");

    // Set weather table elements
    weatherLog->elementAt(1, 0)->addNew<WText>("<div class = 'Welem'> " + weatherInfoDoc[0] + " </div>");
    weatherLog->elementAt(1, 1)->addNew<WText>("<div class = 'Welem'> " + weatherInfoDoc[1] + " </div>");
    weatherLog->elementAt(1, 2)->addNew<WText>("<div class = 'Welem'> " + weatherInfoDoc[2] + " </div>");

    weatherLog->setStyleClass("weatherLogTable");

    // Updates the weather log with the click of a button
    auto result4 = weatherContainer->addNew<WTemplate>();
    result4->setTemplateText("<div> ${updateWeather} </div>");
    auto updateWeather = result4->bindWidget("updateWeather", make_unique<WPushButton>("Update Weather"));
    updateWeather->setStyleClass("weatherButton");
    updateWeather->clicked().connect( [=] {

        // Gets updated file from Raspberry Pi and reads through it
        vector<string> newWeatherInfoDoc;
        fstream newWeatherFile;
        system("sshpass -p password scp -o StrictHostKeyChecking=no pi@192.168.43.139:/home/pi/group3/BME280-Environmental-Sensor-Demo-Code/weathers.txt ~/weathers.txt");
        newWeatherFile.open("/Users/dishapuri/weathers.txt", ios::in);
        if (newWeatherFile.is_open()) {
            // Checks whether the file is open
            string tx;
            // Reads data from file object and put it into string
            while (getline(newWeatherFile, tx, ',')) {
                newWeatherInfoDoc.push_back(tx);
            }
            newWeatherFile.close();
        }

        // Clears the weather table and repopulates it with updated data
        weatherLog->clear();

        weatherLog->elementAt(0, 0)->addNew<WText>("<div class='WheadElem'> <b> Temperature </b> </div>");
        weatherLog->elementAt(0, 1)->addNew<WText>("<div class='WheadElem'> Pressure </div>");
        weatherLog->elementAt(0, 2)->addNew<WText>("<div class='WheadElem'> Humidity </div>");

        weatherLog->elementAt(1, 0)->addNew<WText>("<div class = 'Welem'> " + newWeatherInfoDoc[0] + " </div>");
        weatherLog->elementAt(1, 1)->addNew<WText>("<div class = 'Welem'> " + newWeatherInfoDoc[1] + " </div>");
        weatherLog->elementAt(1, 2)->addNew<WText>("<div class = 'Welem'> " + newWeatherInfoDoc[2] + " </div>");

        });

    checkButtons->addStyleClass("checkButtons");
}

/*!
 \fn main
 @brief Creates an instance of WT environment.
 The main function is the first function to be executed when the security application is compiled. It runs all of the functionalities.
 @param argc Number of arguments
 @param argv Array of arguments
 @return An instance of the displayUI class that you open in your browser.
 @author(s) Disha Puri and Priyanka Bangalore
*/
int main(int argc, char **argv)
{
    return WRun(argc, argv, [](const WEnvironment& env) {
        return make_unique<DisplayUI>(env);
    });
}

