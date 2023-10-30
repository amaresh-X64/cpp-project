#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>
#include <algorithm>
struct Content {
    std::string title;
    std::string description;
    std::string videoFile;
    std::string genre;
    std::string language;
};
struct User {
    std::string username;
    std::string password;
};
bool caseInsensitiveChoiceCompare(const std::string& choice, const std::string& option) {
    return choice.size() == option.size() && std::equal(choice.begin(), choice.end(), option.begin(), option.end(), [](char a, char b) {
        return tolower(a) == tolower(b);
    });
}
bool caseInsensitiveStringCompare(const std::string& str1, const std::string& str2) {
    return str1.size() == str2.size() && std::equal(str1.begin(), str1.end(), str2.begin(), str2.end(), [](char a, char b) {
        return tolower(a) == tolower(b);
    });
}
void displayContent(const std::string& genreFilter, const std::string& languageFilter, const std::vector<Content>& contentDatabase) {
    std::cout << "Available Content:\n";
    for (const Content& content : contentDatabase) {
        if ((genreFilter.empty() || caseInsensitiveStringCompare(content.genre, genreFilter)) && (languageFilter.empty() || caseInsensitiveStringCompare(content.language, languageFilter))) {
            std::cout << content.title << " - " << content.description << " (" << content.genre << ", " << content.language << ")\n";
        }
    }
}
bool loginUser(const std::string& username, const std::string& password, const std::vector<User>& userDatabase) {
    for (const User& user : userDatabase) {
        if (caseInsensitiveStringCompare(user.username, username) && user.password == password) {
            return true;
        }
    }
    return false;
}
int main() {
    std::vector<Content> contentDatabase;
    std::vector<User> userDatabase;
    std::ifstream userFile("users.txt");
    if (!userFile) {
        std::cerr << "Error: Unable to open users file." << std::endl;
        return 1;
    }
    User user;
    while (userFile >> user.username >> user.password) {
        userDatabase.push_back(user);
    }
    userFile.close();
    std::ifstream contentFile("content.txt");
    if (!contentFile) {
        std::cerr << "Error: Unable to open content file." << std::endl;
        return 1;
    }
    Content content;
    std::string line;
    while (std::getline(contentFile, line)) {
        std::istringstream iss(line);
        std::string token;
        std::getline(iss, content.title, ',');
        std::getline(iss, content.description, ',');
        std::getline(iss, content.videoFile, ',');
        std::getline(iss, content.genre, ',');
        std::getline(iss, content.language, ',');
        contentDatabase.push_back(content);
    }
    contentFile.close();
    std::cout << "Welcome to the Case-Insensitive Command-Line OTT Platform!\n";
    while (true) {
        std::cout << "\n1. Display Available Content\n2. Login\n3. Exit\n";
        std::cout << "Enter your choice: ";
        std::string choiceStr;
        std::cin >> choiceStr;
        std::transform(choiceStr.begin(), choiceStr.end(), choiceStr.begin(), ::tolower);
        if (caseInsensitiveChoiceCompare(choiceStr, "1")) {
            std::string genreFilter, languageFilter;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Enter genre (case-insensitive) (or press Enter for all): ";
            std::getline(std::cin, genreFilter);
            std::cout << "Enter language (case-insensitive) (or press Enter for all): ";
            std::getline(std::cin, languageFilter);
            displayContent(genreFilter, languageFilter, contentDatabase);
        } else if (caseInsensitiveChoiceCompare(choiceStr, "2")) {
            std::string username, password;
            std::cout << "Enter username: ";
            std::cin >> username;
            std::cout << "Enter password: ";
            std::cin >> password;
            if (loginUser(username, password, userDatabase)) {
                std::cout << "Login successful! Enjoy the content.\n";
            } else {
                std::cout << "Login failed. Please try again.\n";
            }
        } else if (caseInsensitiveChoiceCompare(choiceStr, "3")) {
            std::cout << "Thank you for using the OTT platform. Goodbye!\n";
            break;
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}
