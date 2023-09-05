#include <iostream>
#include <winsock2.h>
#include <mysql.h>
#include <string>
#include <stdexcept>

using namespace std;

int generateUniqueUserID(MYSQL* conn)
{
    string query = "SELECT MAX(UserID) FROM general";

    int userID = 0;

    int queryResult = mysql_query(conn, query.c_str());
    if (queryResult == 0)
    {
        MYSQL_RES* result = mysql_store_result(conn);

        if (MYSQL_ROW row = mysql_fetch_row(result))
        {
            if (row[0] != nullptr)
            {
                userID = atoi(row[0]);
            }
        }

        mysql_free_result(result);
    }
    else
    {
        cout << "Failed to retrieve maximum UserID: " << mysql_error(conn) << endl;
    }

    return userID + 1;
}

void insertIntoGeneralTable(MYSQL* conn)
{
    int userID = generateUniqueUserID(conn);


    string name;
    string instagramFollowersStr;
    string twitterFollowersStr;
    string youtubeSubscribersStr;

    cout << "Enter Name: ";
    getline(cin, name);

    cout << "Enter Instagram Followers (in M): ";
    getline(cin, instagramFollowersStr);

    cout << "Enter Twitter Followers (in M): ";
    getline(cin, twitterFollowersStr);

    cout << "Enter YouTube Subscribers (in M): ";
    getline(cin, youtubeSubscribersStr);

    string insertQuery = "INSERT INTO general (UserID, Name, InstagramFollowers, TwitterFollowers, YouTubeSubscribers) "
                         "VALUES (" + to_string(userID) + ", '" + name + "', '" +
                         instagramFollowersStr + "', '" +
                         twitterFollowersStr + "', '" +
                         youtubeSubscribersStr + "')";


    // INSERT INTO general (UserID, Name, InstagramFollowers, TwitterFollowers, YouTubeSubscribers)
   // VALUES (userID, 'name', 'instagramFollowers', 'twitterFollowers', 'youtubeSubscribers')


    int queryResult = mysql_query(conn, insertQuery.c_str());
    if (queryResult != 0)
    {
        cout << "Failed to insert values: " << mysql_error(conn) << endl;
        return;
    }

    cout << "Values inserted successfully!" << endl;
    cout << "Generated UserID: " << userID << endl;
}

void updateInstagramData(MYSQL* conn, int userID)
{
    string userName;
    string content;
    cout << "\n--- INSTAGRAM Table Menu ---\n";
    cout << "Enter User Name: ";
    getline(cin, userName);

    cout << "Enter Content: ";
    getline(cin, content);

    // Create the SQL UPDATE statement
    string updateQuery = "UPDATE instagram SET UserName = '" + userName + "', Content = '" + content +
                         "' WHERE UserID = " + to_string(userID);

    int queryResult = mysql_query(conn, updateQuery.c_str());
    if (queryResult != 0)
    {
        cout << "Failed to update values: " << mysql_error(conn) << endl;
        return;
    }

    cout << "Values updated successfully!\n";
}

void updateYouTubeData(MYSQL* conn, int userID)
{
    string channelName;
    string content;


    cout << "\n--- YOUTUBE Table Menu ---\n";
    cout << "Enter Channel Name: ";
    getline(cin, channelName);

    cout << "Enter Content: ";
    getline(cin, content);

    // Create the SQL UPDATE statement
    string updateQuery = "UPDATE youtube SET ChannelName = '" + channelName + "', Content = '" + content +
                         "' WHERE UserID = " + to_string(userID);

    int queryResult = mysql_query(conn, updateQuery.c_str());
    if (queryResult != 0)
    {
        cout << "Failed to update values: " << mysql_error(conn) << endl;
        return;
    }

    cout << "Values updated successfully!\n";
}




void insertIntoInstagramTable(MYSQL* conn)
{
    int userID;
    cout << "\n--- INSTAGRAM Table Menu ---\n";
    cout << "Enter UserID: ";
    cin >> userID;
    cin.ignore();

    // Check if UserID exists in the Instagram table
    string checkQuery = "SELECT COUNT(*) FROM instagram WHERE UserID = " + to_string(userID);

    int queryResult = mysql_query(conn, checkQuery.c_str());
    if (queryResult != 0)
    {
        cout << "Failed to execute query: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (result == nullptr)
    {
        cout << "Failed to get result set: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    int rowCount = atoi(row[0]);

    mysql_free_result(result);

    if (rowCount > 0)
    {
        cout << "UserID already exists in Instagram. Do you want to update the data? (Y/N): ";
        char choice;
        cin >> choice;
        cin.ignore();

        if (choice == 'Y' || choice == 'y')
        {
            updateInstagramData(conn, userID);
            return;
        }
        else
        {
            cout << "Data not updated. Exiting...\n";
            return;
        }
    }

    // Retrieve the number of Instagram followers from the General table
    string followersQuery = "SELECT InstagramFollowers FROM general WHERE UserID = " + to_string(userID);

    queryResult = mysql_query(conn, followersQuery.c_str());
    if (queryResult != 0)
    {
        cout << "Failed to execute query: " << mysql_error(conn) << endl;
        return;
    }

    result = mysql_store_result(conn);
    if (result == nullptr)
    {
        cout << "Failed to get result set: " << mysql_error(conn) << endl;
        return;
    }

    row = mysql_fetch_row(result);
    float followers = atof(row[0]);

    mysql_free_result(result);

    // Continue with inserting data into the Instagram table
    string userName;
    string content;

    cout << "Enter User Name: ";
    getline(cin, userName);

    cout << "Enter Content: ";
    getline(cin, content);

    // Create the SQL INSERT statement
    string insertQuery = "INSERT INTO instagram (UserID, Followers, UserName, Content) "
                         "VALUES (" + to_string(userID) + ", " + to_string(followers) + ", '" +
                         userName + "', '" + content + "')";

    queryResult = mysql_query(conn, insertQuery.c_str());
    if (queryResult != 0)
    {
        cout << "Failed to insert values: " << mysql_error(conn) << endl;
        return;
    }

    cout << "Values inserted successfully!\n";
}

void updateTableData(MYSQL* conn)
{
    int userID;
    cout << "\n--- General Table Menu ---\n";
    cout << "Enter UserID: ";
    cin >> userID;
    cin.ignore();

    // Check if UserID exists in the General table
    string checkQuery = "SELECT COUNT(*) FROM general WHERE UserID = " + to_string(userID);

    int queryResult = mysql_query(conn, checkQuery.c_str());
    if (queryResult != 0)
    {
        cout << "Failed to execute query: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (result == nullptr)
    {
        cout << "Failed to get result set: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    int rowCount = atoi(row[0]);

    mysql_free_result(result);

    if (rowCount == 0)
    {
        cout << "UserID does not exist.\n";
        return;
    }

    string name;
    string instagramFollowersStr;
    string twitterFollowersStr;
    string youtubeSubscribersStr;

    cout << "Enter Name: ";
    getline(cin, name);

    cout << "Enter Instagram Followers (in M): ";
    getline(cin, instagramFollowersStr);

    cout << "Enter Twitter Followers (in M): ";
    getline(cin, twitterFollowersStr);

    cout << "Enter YouTube Subscribers (in M): ";
    getline(cin, youtubeSubscribersStr);

    string updateQuery = "UPDATE general SET Name = '" + name + "', InstagramFollowers = '" +
                         instagramFollowersStr + "', TwitterFollowers = '" +
                         twitterFollowersStr + "', YouTubeSubscribers = '" +
                         youtubeSubscribersStr + "' WHERE UserID = " + to_string(userID);

    queryResult = mysql_query(conn, updateQuery.c_str());
    if (queryResult != 0)
    {
        cout << "Failed to update values: " << mysql_error(conn) << endl;
        return;
    }

    cout << "Values updated successfully!\n";
}

void updateTwitterData(MYSQL* conn, int userID)
{
    string userName;
    string content;
    cout << "\n--- TWITTER Table Menu ---\n";

    cout << "Enter User Name: ";
    getline(cin, userName);

    cout << "Enter Content: ";
    getline(cin, content);

    // Create the SQL UPDATE statement
    string updateQuery = "UPDATE twitter SET UserName = '" + userName + "', Content = '" + content +
                         "' WHERE UserID = " + to_string(userID);

    int queryResult = mysql_query(conn, updateQuery.c_str());
    if (queryResult != 0)
    {
        cout << "Failed to update values: " << mysql_error(conn) << endl;
        return;
    }

    cout << "Values updated successfully!\n";
}

void insertIntoTwitterTable(MYSQL* conn)
{
    int userID;
    cout << "\n--- TWITTER Table Menu ---\n";
    cout << "Enter UserID: ";
    cin >> userID;
    cin.ignore();

    // Check if UserID exists in the Twitter table
    string checkQuery = "SELECT COUNT(*) FROM twitter WHERE UserID = " + to_string(userID);

    int queryResult = mysql_query(conn, checkQuery.c_str());
    if (queryResult != 0)
    {
        cout << "Failed to execute query: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (result == nullptr)
    {
        cout << "Failed to get result set: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    int rowCount = atoi(row[0]);

    mysql_free_result(result);

    if (rowCount > 0)
    {
        cout << "UserID already exists in Twitter. Do you want to update the data? (Y/N): ";
        char choice;
        cin >> choice;
        cin.ignore();

        if (choice == 'Y' || choice == 'y')
        {
            updateTwitterData(conn, userID);
            return;
        }
        else
        {
            cout << "Data not updated. Exiting...\n";
            return;
        }
    }

    // Fetch followers count from the General table
    string followersQuery = "SELECT TwitterFollowers FROM general WHERE UserID = " + to_string(userID);
    queryResult = mysql_query(conn, followersQuery.c_str());
    if (queryResult != 0)
    {
        cout << "Failed to fetch followers count: " << mysql_error(conn) << endl;
        return;
    }

    result = mysql_store_result(conn);
    if (result == nullptr)
    {
        cout << "Failed to get result set: " << mysql_error(conn) << endl;
        return;
    }

    row = mysql_fetch_row(result);
    string twitterFollowersStr = row[0];

    mysql_free_result(result);

    // Convert the Twitter followers count to float
    float twitterFollowers = stof(twitterFollowersStr);

    // Continue with inserting data into the Twitter table
    string userName;
    string content;

    cout << "Enter User Name: ";
    getline(cin, userName);

    cout << "Enter Content: ";
    getline(cin, content);

    // Create the SQL INSERT statement
    string insertQuery = "INSERT INTO twitter (UserID, Followers, UserName, Content) "
                         "VALUES (" + to_string(userID) + ", " + to_string(twitterFollowers) + ", '" +
                         userName + "', '" + content + "')";

    queryResult = mysql_query(conn, insertQuery.c_str());
    if (queryResult != 0)
    {
        cout << "Failed to insert values: " << mysql_error(conn) << endl;
        return;
    }

    cout << "Values inserted successfully!\n";
}




void insertIntoYouTubeTable(MYSQL* conn)
{
    int userID;
    cout << "\n--- YOUTUBE Table Menu ---\n";
    cout << "Enter UserID: ";
    cin >> userID;
    cin.ignore();

    // Check if UserID exists in the General table
    string checkQuery = "SELECT COUNT(*) FROM general WHERE UserID = " + to_string(userID);

    int queryResult = mysql_query(conn, checkQuery.c_str());
    if (queryResult != 0)
    {
        cout << "Failed to execute query: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (result == nullptr)
    {
        cout << "Failed to get result set: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    int rowCount = atoi(row[0]);

    mysql_free_result(result);

    if (rowCount == 0)
    {
        cout << "No valid UserID found.\n";
        return;
    }

    // Retrieve the number of YouTube subscribers from the General table
    string subscribersQuery = "SELECT YouTubeSubscribers FROM general WHERE UserID = " + to_string(userID);

    queryResult = mysql_query(conn, subscribersQuery.c_str());
    if (queryResult != 0)
    {
        cout << "Failed to execute query: " << mysql_error(conn) << endl;
        return;
    }

    result = mysql_store_result(conn);
    if (result == nullptr)
    {
        cout << "Failed to get result set: " << mysql_error(conn) << endl;
        return;
    }

    row = mysql_fetch_row(result);
    float subscribers = atof(row[0]);

    mysql_free_result(result);

    // Continue with inserting data into the YouTube table
    string channelName;
    string content;

    cout << "Enter Channel Name: ";
    getline(cin, channelName);

    cout << "Enter Content: ";
    getline(cin, content);

    // Create the SQL INSERT statement
    string insertQuery = "INSERT INTO youtube (UserID, Subscribers, ChannelName, Content) "
                         "VALUES (" + to_string(userID) + ", " + to_string(subscribers) + ", '" +
                         channelName + "', '" + content + "')";

    queryResult = mysql_query(conn, insertQuery.c_str());
    if (queryResult != 0)
    {
        cout << "Failed to insert values: " << mysql_error(conn) << endl;
        return;
    }

    cout << "Values inserted successfully!\n";
}

void deleteUserData(MYSQL* conn, int userID)
{
    // Delete data from the Instagram table
    string deleteQueryInstagram = "DELETE FROM instagram WHERE UserID = " + to_string(userID);
    int queryResultInstagram = mysql_query(conn, deleteQueryInstagram.c_str());
    if (queryResultInstagram != 0)
    {
        cout << "Failed to delete data from Instagram table: " << mysql_error(conn) << endl;
        return;
    }

    // Delete data from the Twitter table
    string deleteQueryTwitter = "DELETE FROM twitter WHERE UserID = " + to_string(userID);
    int queryResultTwitter = mysql_query(conn, deleteQueryTwitter.c_str());
    if (queryResultTwitter != 0)
    {
        cout << "Failed to delete data from Twitter table: " << mysql_error(conn) << endl;
        return;
    }

    // Delete data from the YouTube table
    string deleteQueryYouTube = "DELETE FROM youtube WHERE UserID = " + to_string(userID);
    int queryResultYouTube = mysql_query(conn, deleteQueryYouTube.c_str());
    if (queryResultYouTube != 0)
    {
        cout << "Failed to delete data from YouTube table: " << mysql_error(conn) << endl;
        return;
    }

    // Delete data from the General table
    string deleteQueryGeneral = "DELETE FROM general WHERE UserID = " + to_string(userID);
    int queryResultGeneral = mysql_query(conn, deleteQueryGeneral.c_str());
    if (queryResultGeneral != 0)
    {
        cout << "Failed to delete data from General table: " << mysql_error(conn) << endl;
        return;
    }

    cout << "User data deleted successfully!\n";
}
void selectTableToAddData(MYSQL* conn)
{
    string choice;

    while (true)
    {
        cout << "Select a table to add/update/delete data:\n";
        cout << "1. General\n";
        cout << "2. Instagram\n";
        cout << "3. YouTube\n";
        cout << "4. Twitter\n";
        cout << "Enter the corresponding number or type 'exit' to exit: ";
        getline(cin, choice);

        if (choice == "exit")
            break;

        int tableChoice = stoi(choice);

        switch (tableChoice)
        {
            case 1:
            {
                while (true)
                {
                    cout << "\n--- General Table Menu ---\n";
                    cout << "1. Create New User\n";
                    cout << "2. Update User Data\n";
                    cout << "3. Delete User Data\n";
                    cout << "Enter the corresponding number or type 'back' to go back: ";
                    getline(cin, choice);

                    if (choice == "back")
                        break;

                    int operationChoice = stoi(choice);

                    switch (operationChoice)
                    {
                        case 1:
                            insertIntoGeneralTable(conn);
                            break;
                        case 2:
                            updateTableData(conn);
                            break;
                        case 3:
                            cout << "Enter the UserID to delete: ";
                            getline(cin, choice);
                            int userID = stoi(choice);
                            deleteUserData(conn, userID);
                            break;

                    }
                }
                break;
            }
            case 2:
                insertIntoInstagramTable(conn);
                break;
            case 3:
                insertIntoYouTubeTable(conn);
                break;
            case 4:
                insertIntoTwitterTable(conn);
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    }
}
// Function to fetch and display information based on followers' range
void promoteAd(MYSQL* conn)
{
    string platformChoice;
    string minFollowersStr, maxFollowersStr;
    float minFollowers, maxFollowers;

    cout << "Which platform do you want to promote?\n";
    cout << "1. Instagram\n";
    cout << "2. Twitter\n";
    cout << "3. YouTube\n";
    cout << "Enter the corresponding number: ";
    getline(cin, platformChoice);

    string tableName; // To store the selected table name

    switch (platformChoice[0])
    {
        case '1':
            tableName = "instagram";
            break;
        case '2':
            tableName = "twitter";
            break;
        case '3':
            tableName = "youtube";
            break;
        default:
            cout << "Invalid platform choice. Promotion failed.\n";
            return;
    }

    cout << "Enter the minimum number of followers/subscribers: ";
    getline(cin, minFollowersStr);

    cout << "Enter the maximum number of followers/subscribers: ";
    getline(cin, maxFollowersStr);

    // Convert input strings to floats
    minFollowers = stof(minFollowersStr);
    maxFollowers = stof(maxFollowersStr);

    // Define column names for YouTube table
    string followersColumn, usernameColumn;
    if (tableName == "youtube")
    {
        followersColumn = "Subscribers";
        usernameColumn = "ChannelName";
    }
    else
    {
        // For Instagram and Twitter, continue using "Followers" and "Username" columns
        followersColumn = "Followers";
        usernameColumn = "Username";
    }

    // Query to fetch information from the selected table based on followers' range
    string selectQuery = "SELECT UserID, " + followersColumn + ", " + usernameColumn + ", Content FROM " + tableName
                        + " WHERE " + followersColumn + " >= " + to_string(minFollowers)
                        + " AND " + followersColumn + " <= " + to_string(maxFollowers);

    int queryResult = mysql_query(conn, selectQuery.c_str());

    if (queryResult != 0)
    {
        cout << "Failed to fetch data from the table: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* result = mysql_store_result(conn);

    if (result)
    {
        // Fetch and display data from the table
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result)))
        {
            int userID = atoi(row[0]);
            float followers = atof(row[1]);
            string username = row[2];
            string content = row[3];

            cout << "UserID: " << userID << endl;
            cout << "Followers/Subscribers: " << followers <<"M"<< endl;
            cout << "Username/ChannelName: " << username << endl;
            cout << "Content: " << content << endl;
            cout << "---------------------------" << endl;
        }

        mysql_free_result(result);
    }
    else
    {
        cout << "No data found in the " << tableName << " table for the specified followers' range." << endl;
    }
}


int main()
{
    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "192.168.29.196", "aditya", "15011", "reach handling", 0, NULL, 0);

    if (conn == NULL)
    {
        cout << "Failed to connect to database: " << mysql_error(conn) << endl;
        return 1;
    }

    bool proceed = true;
    while (proceed)
    {
        cout << "Choose an option:\n";
        cout << "1. Select a table to add/update/delete data\n";
        cout << "2. Promote Ad\n";
        cout << "3. Exit\n";
        cout << "Enter the corresponding number: ";
        char choice;
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
            case '1':
                selectTableToAddData(conn);
                break;
            case '2':
                promoteAd(conn);
                break;
            case '3':
                proceed = false;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    }

    mysql_close(conn);

    return 0;
}
