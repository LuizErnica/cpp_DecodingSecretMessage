#include <iostream>
#include <string>
#include <map>
#include <curl/curl.h>

using namespace std;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output)
{
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string downloadHTML(const string& url)
{
    CURL* curl = curl_easy_init();
    string buffer;

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return buffer;
}

string getNextLetterValue(string &html)
{
	string ret;
	basic_string <char>::size_type index;
	basic_string <char>::size_type index2;

	index = html.find("<span class="); // This token is the beggining of the characteres coordinates lines in the HTML code
	if (index == string::npos)
		ret = "";

	index = html.find(">", index); // This token is the beggining of the data value in the HTML code
	if (index == string::npos)
		ret = "";

	index2 = html.find("</span>", index); // This token is the end of the HTML code for the data value in the HTML code
	if (index2 == string::npos)
		ret = "";

	ret = html.substr(index + 1, index2 - index - 1);
	html = html.substr(index2);

	return ret;
}
int main()
{
    string url, html;
	map<int, map<int, string>> lettersMap; // This map will store the coordinates and the corresponding character, where the key of the outer map is the x coordinate and the key of the inner map is the y coordinate
    int x, y;
    string ret, letter;

	//cout << "Enter the URL: ";
    //getline(cin, url);

	url = "https://docs.google.com/document/d/e/2PACX-1vTMOmshQe8YvaRXi6gEPKKlsC6UpFJSMAk4mQjLm_u1gmHdVVTaeh7nBNFBRlui0sTZ-snGwZM4DBCT/pub";

	html = downloadHTML(url);

	// This token is the beggining of the characteres coordinates table in the HTML code
	html = html.substr(html.find("y-coordinate"));

	do
	{
		// Get X coordinate
		if ((ret = getNextLetterValue(html)) == "")
			break;

		x = stoi(ret);

		// Get Letter
		if ((letter = getNextLetterValue(html)) == "")
			break;

		// Get Y coordinate
		if ((ret = getNextLetterValue(html)) == "")
			break;

		y = stoi(ret);

		lettersMap[y][x] = letter; // Store the letter in the map with its coordinates as keys

	} while (!html.empty());

	for (int yid = lettersMap.size() - 1; yid >= 0 ; yid--)
	{
		for (int xid = 0 ; xid < lettersMap[yid].size() - 1; xid++)
		{
			letter = lettersMap[yid][xid];

			if (letter == "")
				cout << " ";
			else
				cout << letter;
		}

		cout << endl;
	}

    return 0;
}
/* 

issues:
	01 - missing last letters in the map
	02 - Need to print SPACE when there is less letters in the template because the map does not compute spaces.
* */
