#include <string>
#include <vector>

using namespace std;

vector<string> split(string str, const char* separator) {
    int i = 0, startIndex = 0;
    vector<string> words;

    while (i <= (int)str.length() + 1) {
        if (i == (int)str.length() + 1 || str[i] == *separator) {
            string subString = "";
            subString.append(str, startIndex, i - startIndex);

            words.push_back(subString);

            startIndex = i + 1;
        }

        i += 1;
    }

    return words;
}
