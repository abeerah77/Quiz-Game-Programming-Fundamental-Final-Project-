#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

const int maxquestions = 50;
const int maxoptions = 4;
const int quizsize = 10;
const int maxplayers = 100;
//Arrays for Questions
string questiontext[maxquestions];
string questionoption1[maxquestions];
string questionoption2[maxquestions];
string questionoption3[maxquestions];
string questionoption4[maxquestions];
int questionanswer[maxquestions];
int questiondifficulty[maxquestions];
//Arrays for Selected Quiz Questions
string selectedtext[quizsize];
string selectedoption1[quizsize];
string selectedoption2[quizsize];
string selectedoption3[quizsize];
string selectedoption4[quizsize];
int selectedanswer[quizsize];
//Arrays for Wrong Answers Review
string wrongtext[quizsize];
string wrongoption1[quizsize];
string wrongoption2[quizsize];
string wrongoption3[quizsize];
string wrongoption4[quizsize];
int wronganswer[quizsize];
int wrongcount = 0;
//Arrays for Leaderboard
string playernames[maxplayers];
int playerscores[maxplayers];
string playerdates[maxplayers];
string playercategories[maxplayers];
// Game State Variables
string currentplayername;
int currentscore = 0;
int correctcount = 0;
int wrongcountstate = 0;
int currentquestionnum = 0;
int currentdifficulty = 1;
string currentcategory;
bool lifeline5050 = true;
bool lifelineskip = true;
bool lifelinereplace = true;
bool lifelineextratime = true;
int currentstreak = 0;
//Convert integerer number to string becoz text is strored in file in form of text(string)
string intToString(int n) {
    if (n == 0) return "0";
    
    string s = "";
    int num = n;
    
    if (num < 0) num = -num;
    
    while (num > 0) {
        char c = '0' + (num % 10);
        s = c + s;
        num /= 10;
    }
    
    if (n < 0) s = "-" + s;
    
    return s;
}
//To get date from computer and display 
string getcurrentdate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    string day = intToString(ltm->tm_mday);
    string month = intToString(ltm->tm_mon + 1);
    string year = intToString(ltm->tm_year + 1900);
    
    return day + "/" + month + "/" + year;
}
//to get time 
string getcurrenttime() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    string hour = intToString(ltm->tm_hour);
    string min = intToString(ltm->tm_min);
    
    if (ltm->tm_min < 10) {
        min = "0" + min;
    }
    
    return hour + ":" + min;
}
//used to print line when ever this function is called
void printline(int length, char ch) {
    for (int i = 0; i < length; i++) cout << ch;
    cout << endl;
}
//this is to make title look decorative
void printheader(string title) {
    cout << endl;
    printline(60, '=');
    cout << "    " << title << endl;
    printline(60, '=');
    cout << endl;
}
//In our code, at some place 3 is the minimun menu options, at some places it exceeds so ...
void printmenuoptions(string opt1, string opt2, string opt3, 
                      string opt4, string opt5, string opt6, int count) {
    cout << "  [1] " << opt1 << endl;
    cout << "  [2] " << opt2 << endl;
    cout << "  [3] " << opt3 << endl;
    if (count >= 4) cout << "  [4] " << opt4 << endl;
    if (count >= 5) cout << "  [5] " << opt5 << endl;
    if (count >= 6) cout << "  [6] " << opt6 << endl;
    cout << endl;
}
//this function read the question from the file and store it in the arrays that we declare above.
//and that contain total questions present in the file.
//it shows total number of questiosn at the end. 
int loadquestions(string filename, int difficultylevel) {
    ifstream file(filename.c_str());
    if (!file.is_open()) {
        cout << "  [ERROR] Could not open file: " << filename << endl;
        return 0;
    }
    
    int count = 0;
    string temptext;
    string tempoption1, tempoption2, tempoption3, tempoption4;
    int tempanswer;
    int tempdiff;
    
    while (count < maxquestions && getline(file, temptext)) {
        if (temptext.empty()) continue;
        
        getline(file, tempoption1);
        getline(file, tempoption2);
        getline(file, tempoption3);
        getline(file, tempoption4);
        file >> tempanswer;
        file >> tempdiff;
        file.ignore();
                
        if (difficultylevel == 0 || tempdiff == difficultylevel) {
            questiontext[count] = temptext;
            questionoption1[count] = tempoption1;
            questionoption2[count] = tempoption2;
            questionoption3[count] = tempoption3;
            questionoption4[count] = tempoption4;
            questionanswer[count] = tempanswer;
            questiondifficulty[count] = tempdiff;
            count++;
        }
    }
    
    file.close();
    return count;
}
//then we use this function to shuffle the indexes. and hence the questions. for ex; 
//if we have questiontext[0], it can be replaced with questiontext[j=any number];  
void shufflearray(int arr[], int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}
//This function shows question on screen along with 4 options, if user uses 50/50 lifeline it hides two options.
void displayquestion(int qindex, int qnum, bool use5050, int hidden1, int hidden2) {
    cout << endl;
    printline(60, '-');
    cout << "  Question " << qnum << ":" << endl;
    cout << endl;
    cout << "  " << selectedtext[qindex] << endl;
    cout << endl;
    
    if (use5050 && (hidden1 == 0 || hidden2 == 0)) {
        cout << "  [1] --------" << endl;
    } else {
        cout << "  [1] " << selectedoption1[qindex] << endl;
    }
    
    if (use5050 && (hidden1 == 1 || hidden2 == 1)) {
        cout << "  [2] --------" << endl;
    } else {
        cout << "  [2] " << selectedoption2[qindex] << endl;
    }
    
    if (use5050 && (hidden1 == 2 || hidden2 == 2)) {
        cout << "  [3] --------" << endl;
    } else {
        cout << "  [3] " << selectedoption3[qindex] << endl;
    }
    
    if (use5050 && (hidden1 == 3 || hidden2 == 3)) {
        cout << "  [4] --------" << endl;
    } else {
        cout << "  [4] " << selectedoption4[qindex] << endl;
    }
    
    cout << endl;
    printline(60, '-');
}
//it shows lifelines on screen. if there's no any, it displays none.
void displaylifelines() {
    cout << "  Available Lifelines: ";
    if (lifeline5050) cout << "[5] 50/50 ";
    if (lifelineskip) cout << "[6] Skip ";
    if (lifelinereplace) cout << "[7] Replace ";
    if (lifelineextratime) cout << "[8] Extra Time ";
    if (!lifeline5050 && !lifelineskip && !lifelinereplace && !lifelineextratime) {
        cout << "None";
    }
    cout << endl;
}
//it counts all the wrong options and then tells us which two wrong options to hide. 
void apply5050(int qindex, int& hidden1, int& hidden2) {
    int wrongopts[3];
    int wcount = 0;
    for (int i = 0; i < maxoptions; i++) {
        if (i != selectedanswer[qindex] - 1) {
            wrongopts[wcount++] = i;  //wrongopts[0]=0,wrongopts[1]=2,...
        }
    }
    //hide two random options now
    int idx1 = rand() % wcount; //0,1,2
    int idx2;
    do {
        idx2 = rand() % wcount;
    } while (idx2 == idx1);
    
    hidden1 = wrongopts[idx1];
    hidden2 = wrongopts[idx2];
}
//to calculate negative marking
int calculatenegativemarking(int difficulty) {
    if (difficulty == 1) return 2;
    if (difficulty == 2) return 3;
    return 5;
}
//it calculates streakbonus
int calculatestreakbonus(int streak) {
    if (streak >= 5) return 15;
    if (streak >= 3) return 5;
    return 0;
}
//if user enter the wrong question during the quiz 
//then this function is used to display the correct text option. 
string getSelectedOption(int qindex, int optnum) {
    if (optnum == 1) return selectedoption1[qindex];
    if (optnum == 2) return selectedoption2[qindex];
    if (optnum == 3) return selectedoption3[qindex];
    return selectedoption4[qindex];
}
//it is used at the end to review the wrong options. 
string getWrongOption(int qindex, int optnum) {
    if (optnum == 1) return wrongoption1[qindex];
    if (optnum == 2) return wrongoption2[qindex];
    if (optnum == 3) return wrongoption3[qindex];
    return wrongoption4[qindex];
}
//to store high score data in the file without changing the previous one.
void savehighscore(string name, int score, string category) {
    ofstream file("highscores.txt", ios::app);
    if (file.is_open()) {
        file << name << "|" << score << "|" << getcurrentdate() << "|" << category << endl;
        file.close();
    }
}
// to convert from int to string
int stringToInt(string str) {
    int result = 0;
    
    for (int i = 0; i < str.length(); i++) {
        char c = str[i];
        if (c >= '0' && c <= '9') {
            result = result * 10 + (c - '0');
        }
    }
    return result;
}
//read highscores from the file
int loadhighscores() {
    ifstream file("highscores.txt");
    int count = 0;
    
    if (!file.is_open()) return 0;
    
    string line;
    while (count < maxplayers && getline(file, line)) {
        if (line.empty()) continue;
        
        string parts[4];
        int partIdx = 0;
        string temp = "";
        
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == '|') {
                if (partIdx < 4) parts[partIdx++] = temp;
                temp = "";
            } else {
                temp += line[i];
            }
        }
        if (partIdx < 4) parts[partIdx] = temp;
        
        playernames[count] = parts[0];
        playerscores[count] = stringToInt(parts[1]);
        playerdates[count] = parts[2];
        playercategories[count] = parts[3];
        
        count++;
    }
    
    file.close();
    return count;
}

void displayleaderboard() {
	// Load high scores from file into arrays
    int count = loadhighscores();
    // BUBBLE SORTING TO SHOW Highest Score First
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
        	// If the score at j is smaller than score at j+1, swap them
            if (playerscores[j] < playerscores[j + 1]) {
                int tempscore = playerscores[j];
                playerscores[j] = playerscores[j + 1];
                playerscores[j + 1] = tempscore;
                
                string tempname = playernames[j];
                playernames[j] = playernames[j + 1];
                playernames[j + 1] = tempname;
                
                string tempdate = playerdates[j];
                playerdates[j] = playerdates[j + 1];
                playerdates[j + 1] = tempdate;
                
                string tempcat = playercategories[j];
                playercategories[j] = playercategories[j + 1];
                playercategories[j + 1] = tempcat;
            }
        }
    }
    
    printheader("LEADERBOARD - TOP 5 SCORES");
    cout << "  Rank  Name                Score    Date         Category" << endl;
    printline(60, '-');
    
    int displaycount = (count < 5) ? count : 5;
    for (int i = 0; i < displaycount; i++) {
        cout << "  " << (i + 1) << ".    ";
        cout << playernames[i];
        //Align names neatly by adding spaces
        int padding = 20 - playernames[i].length();
        if (padding < 0) padding = 0;
        for (int j = 0; j < padding; j++) cout << " ";
        
        cout << playerscores[i];
        
        int scorelen = 1;
        int tempS = playerscores[i];
        while (tempS > 9) { tempS /= 10; scorelen++; }
        
        padding = 8 - scorelen;
        if (padding < 0) padding = 0;
        for (int j = 0; j < padding; j++) cout << " ";
        
        cout << playerdates[i] << "  " << playercategories[i] << endl;
    }
    
    if (count == 0) {
        cout << "  No high scores yet. Be the first to play!" << endl;
    }
    cout << endl;
}
//SAVE QUIZ DATA TO THE FILE
void savequizlog(string name, int score, int correct, int wrong, string category) {
    ofstream file("quizlogs.txt", ios::app);
    if (file.is_open()) {
        file << "========================================" << endl;
        file << "Player: " << name << endl;
        file << "Date: " << getcurrentdate() << " Time: " << getcurrenttime() << endl;
        file << "Category: " << category << endl;
        file << "Score: " << score << endl;
        file << "Correct: " << correct << " | Wrong: " << wrong << endl;
        
        if (wrongcount > 0) {
            file << "Questions Answered Incorrectly:" << endl;
            for (int i = 0; i < wrongcount; i++) {
                file << "  - " << wrongtext[i] << endl;
                file << "    Correct Answer: " << getWrongOption(i, wronganswer[i]) << endl;
            }
        }
        
        file << "========================================" << endl << endl;
        file.close();
    }
}
//to Review questions at the end
void reviewwrongquestions() {
    if (wrongcount == 0) {
        cout << "  Congratulations! You answered all questions correctly!" << endl;
        return;
    }
    
    printheader("REVIEW - INCORRECT ANSWERS");
    for (int i = 0; i < wrongcount; i++) {
        cout << "  Question " << (i + 1) << ": " << wrongtext[i] << endl;
        cout << "  Correct Answer: " << getWrongOption(i, wronganswer[i]) << endl;
        cout << endl;
    }
}
//to select category
int selectcategory() {
    printheader("SELECT CATEGORY");
    printmenuoptions("Science", "Computer", "Sports", "History", "IQ/Logic", "", 5);
    cout << "  Enter your choice: ";
    int choice;
    cin >> choice;
    if (choice < 1 || choice > 5) choice = 1;
    return choice;
}
//Function to select difficulty level
int selectdifficulty() {
    printheader("SELECT DIFFICULTY");
    printmenuoptions("Easy (General Knowledge)", "Medium (Moderate)", "Hard (Advanced)", "", "", "", 3);
    cout << "  Enter your choice: ";
    int choice;
    cin >> choice;
    if (choice < 1 || choice > 3) choice = 1;
    return choice;
}
//Select category file; science, computer, sports, history and open file
string getcategoryfilename(int category) {
    if (category == 1) return "science.txt";
    if (category == 2) return "computer.txt";
    if (category == 3) return "sports.txt";
    if (category == 4) return "history.txt";
    if (category == 5) return "iq.txt";
    return "science.txt";
}
//Select category
string getcategoryname(int category) {
    if (category == 1) return "Science";
    if (category == 2) return "Computer";
    if (category == 3) return "Sports";
    if (category == 4) return "History";
    if (category == 5) return "IQ";
    return "General";
}
//After one round, reset game
void resetgamestate() {
    currentscore = 0;
    correctcount = 0;
    wrongcountstate = 0;
    currentquestionnum = 0;
    lifeline5050 = true;
    lifelineskip = true;
    lifelinereplace = true;
    lifelineextratime = true;
    currentstreak = 0;
    wrongcount = 0;
}
//To store 10 questions, 10 options, 10 answers from all the questions that is read from the files. 
void copyQuestionToSelected(int srcIdx, int destIdx) {
    selectedtext[destIdx] = questiontext[srcIdx];
    selectedoption1[destIdx] = questionoption1[srcIdx];
    selectedoption2[destIdx] = questionoption2[srcIdx];
    selectedoption3[destIdx] = questionoption3[srcIdx];
    selectedoption4[destIdx] = questionoption4[srcIdx];
    selectedanswer[destIdx] = questionanswer[srcIdx];
}
//To store wrong questions, options, answers
void saveWrongQuestion(int qindex) {
    wrongtext[wrongcount] = selectedtext[qindex];
    wrongoption1[wrongcount] = selectedoption1[qindex];
    wrongoption2[wrongcount] = selectedoption2[qindex];
    wrongoption3[wrongcount] = selectedoption3[qindex];
    wrongoption4[wrongcount] = selectedoption4[qindex];
    wronganswer[wrongcount] = selectedanswer[qindex];
    wrongcount++;
}
// FUNCTION: playquiz()
// Main quiz engine — handles lifelines, questioning, scoring, timing
void playquiz() {
    wrongcount = 0;
    resetgamestate(); // Reset score, streak, etc.
    
    cout << "  Enter your name: ";
    cin.ignore(1000, '\n');
    getline(cin, currentplayername);
    
    int categorynum = selectcategory(); 
    currentcategory = getcategoryname(categorynum); // Choose category
    
    string filename = getcategoryfilename(categorynum);
    currentdifficulty = selectdifficulty(); // Choose difficulty
    
    int totalloaded = loadquestions(filename, currentdifficulty);
    if (totalloaded < quizsize) {
        cout << "  [WARNING] Not enough questions for difficulty level." << endl;
        cout << "  Loading all available questions..." << endl;
        totalloaded = loadquestions(filename, 0);
    }
    
    if (totalloaded == 0) {
        cout << "  [ERROR] No questions available. Please check question files." << endl;
        return;
    }
    // Create array of indices and shuffle them
    int indices[maxquestions];
    for (int i = 0; i < totalloaded; i++) indices[i] = i;
    shufflearray(indices, totalloaded);
    // Select first quizsize shuffled questions
    int selectcount = (totalloaded < quizsize) ? totalloaded : quizsize;
    for (int i = 0; i < selectcount; i++) {
        copyQuestionToSelected(indices[i], i);
    }
    
    printheader("QUIZ STARTED");
    cout << "  Player: " << currentplayername << endl;
    cout << "  Category: " << currentcategory << endl;
    cout << "  Difficulty: ";
    if (currentdifficulty == 1) cout << "Easy";
    else if (currentdifficulty == 2) cout << "Medium";
    else cout << "Hard";
    cout << endl << endl;
    
    int timelimit = 15;
    int negativemarking = calculatenegativemarking(currentdifficulty);
    //LOOP THROUGH QUESTIONS
    for (int i = 0; i < quizsize; i++) {
        currentquestionnum = i;
        
        bool usedlifeline5050 = false;
        int hidden1 = -1, hidden2 = -1;
        int currenttimelimit = timelimit;
        bool answered = false;
        // REPEAT UNTIL VALID ANSWER GIVEN
        while (!answered) {
            displayquestion(i, i + 1, usedlifeline5050, hidden1, hidden2);
            displaylifelines();
            
            cout << "  Current Score: " << currentscore;
            cout << " | Streak: " << currentstreak << endl;
            cout << "  You have " << currenttimelimit << " seconds to answer!" << endl;
            cout << "  Enter your answer (1-4) or lifeline (5-8): ";
            
            int answer;
            cin >> answer;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "  [INVALID] Please enter a number." << endl;
                continue;
            }
            
            if (answer >= 5 && answer <= 8) {
                if (answer == 5 && lifeline5050) {
                    apply5050(i, hidden1, hidden2);
                    lifeline5050 = false;
                    usedlifeline5050 = true;
                    cout << "  [LIFELINE] 50/50 used! Two wrong options removed." << endl;
                } else if (answer == 6 && lifelineskip) {
                    lifelineskip = false;
                    cout << "  [LIFELINE] Question skipped!" << endl;
                    answered = true; 
                    currentstreak = 0;
                } else if (answer == 7 && lifelinereplace) {
                    lifelinereplace = false;
                    cout << "  [LIFELINE] Question replaced (Simulated)!" << endl;
                    answered = true;
                } else if (answer == 8 && lifelineextratime) {
                    currenttimelimit += 10;
                    lifelineextratime = false;
                    cout << "  [LIFELINE] Extra time added!" << endl;
                } else {
                    cout << "  [ERROR] Lifeline already used or invalid." << endl;
                }
            } else if (answer >= 1 && answer <= 4) {
                if (answer == selectedanswer[i]) {
                    cout << "  [CORRECT] Well done!" << endl;
                    currentscore += 10;
                    correctcount++;
                    currentstreak++;
                    
                    int bonus = calculatestreakbonus(currentstreak);
                    if (bonus > 0) {
                        cout << "  [STREAK BONUS] +" << bonus << " points for ";
                        cout << currentstreak << " correct answers!" << endl;
                        currentscore += bonus;
                    }
                } else {
                    cout << "  [WRONG] The correct answer was: ";
                    cout << getSelectedOption(i, selectedanswer[i]) << endl;
                    currentscore -= negativemarking;
                    wrongcountstate++;
                    currentstreak = 0;
                    saveWrongQuestion(i);
                }
                answered = true;
            } else {
                cout << "  [INVALID] Please enter a valid option (1-4) or lifeline (5-8)." << endl;
            }
        }
        
        cout << endl;
        cout << "  Press Enter to continue...";
        cin.ignore(1000, '\n');
        cin.get();
    }
    
    printheader("QUIZ COMPLETED");
    cout << "  Player: " << currentplayername << endl;
    cout << "  Category: " << currentcategory << endl;
    cout << "  Final Score: " << currentscore << endl;
    cout << "  Correct Answers: " << correctcount << "/" << quizsize << endl;
    cout << "  Wrong Answers: " << wrongcountstate << endl;
    cout << endl;
    
    savehighscore(currentplayername, currentscore, currentcategory);
    savequizlog(currentplayername, currentscore, correctcount, wrongcountstate, currentcategory);
    
    cout << "  Would you like to review incorrect answers? (1=Yes, 2=No): ";
    int reviewchoice;
    cin >> reviewchoice;
    
    if (reviewchoice == 1) {
        reviewwrongquestions();
    }
}
//Allows user to submit a new question which is saved into a file
void suggestquestion() {
    printheader("SUGGEST A NEW QUESTION");
    
    cout << "  Enter your question: " << endl;
    cout << "  > ";
    cin.ignore(1000, '\n');
    string questiontxt;
    getline(cin, questiontxt);
    
    string options[4];
    cout << "  Enter Option 1: ";
    getline(cin, options[0]);
    cout << "  Enter Option 2: ";
    getline(cin, options[1]);
    cout << "  Enter Option 3: ";
    getline(cin, options[2]);
    cout << "  Enter Option 4: ";
    getline(cin, options[3]);
    
    cout << "  Enter correct answer (1-4): ";
    int correct;
    cin >> correct;
    
    cout << "  Enter difficulty (1=Easy, 2=Medium, 3=Hard): ";
    int diff;
    cin >> diff;
    
    cout << "  Select category (1=Science, 2=Computer, 3=Sports, 4=History, 5=IQ): ";
    int cat;
    cin >> cat;
    
    ofstream file("suggestedquestions.txt", ios::app);
    if (file.is_open()) {
        string categoryname = getcategoryname(cat);
        
        file << "Category: " << categoryname << endl;
        file << questiontxt << endl;
        for (int i = 0; i < 4; i++)
            file << options[i] << endl;
        file << correct << endl;
        file << diff << endl;
        file << "---" << endl;
        file.close();
        
        cout << endl;
        cout << "  [SUCCESS] Your question has been submitted for admin review!" << endl;
    } else {
        cout << "  [ERROR] Could not save question. Please try again." << endl;
    }
}
// MAIN FUNCTION
int main() {
    srand(time(0));
    
    int choice;
    bool running = true;
    
    while (running) {
        printheader("CONSOLE QUIZ GAME");
        printmenuoptions("Start New Quiz", "View Leaderboard", "Suggest a Question", "Exit Game", "", "", 4);
        
        cout << "  Enter your choice: ";
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "  [ERROR] Please enter a valid number." << endl;
            continue;
        }
        
        if (choice == 1) {
            playquiz();
        } else if (choice == 2) {
            displayleaderboard();
            cout << "  Press Enter to continue...";
            cin.ignore(1000, '\n');
            cin.get();
        } else if (choice == 3) {
            suggestquestion();
            cout << "  Press Enter to continue...";
            cin.ignore(1000, '\n');
            cin.get();
        } else if (choice == 4) {
            cout << endl;
            cout << "  Thank you for playing! Goodbye." << endl;
            cout << endl;
            running = false;
        } else {
            cout << "  [ERROR] Invalid option. Please try again." << endl;
        }
    }
    
    return 0;
}




















