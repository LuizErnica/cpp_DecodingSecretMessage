#include <iostream>
#include <string>
#include <map>
#include <curl/curl.h>

using namespace std;

// Callback function to write the received HTML data into a string
size_t static writeCallback(void* contents, size_t size, size_t nmemb, string *output)
{
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to fetch the HTML content of a given URL using libcurl
string static getHTML(const string &url)
{
    CURL* curl = curl_easy_init();
    string htmlBuffer;

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &htmlBuffer);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return htmlBuffer;
}

// Function to extract the next letter value from the HTML code, which is located between <span> and </span> tags
string static getNextLetterValue(string &html)
{
	string ret;
	basic_string <char>::size_type index;
	basic_string <char>::size_type index2;

	index = html.find("<span"); // This token is the beggining of the data value in the HTML code
	if (index == string::npos)
		ret = "";

	index = html.find(">", index); // This token is the beggining of the data value in the HTML code
	if (index == string::npos)
		ret = "";

	index2 = html.find("</span>", index); // This token is the end of the data value in the HTML code
	if (index2 == string::npos)
		ret = "";

	ret = html.substr(index + 1, index2 - index - 1); // Get the data value from the HTML code
	html = html.substr(index2); // Update the HTML code to remove the already extracted data value, so the next call to this function will extract the next data value

	return ret;
}

// Main function to execute the program
int main()
{
    string url, html;
	map<int, map<int, string>> lettersMap; // This map will store the coordinates and the corresponding character (Y, X, Letter)
    int y, x;
    string ret, letter;

	cout << "Enter the URL: ";
    getline(cin, url);

	/* Example URLs to test:
	url = "https://docs.google.com/document/d/e/2PACX-1vTMOmshQe8YvaRXi6gEPKKlsC6UpFJSMAk4mQjLm_u1gmHdVVTaeh7nBNFBRlui0sTZ-snGwZM4DBCT/pub";
	url = "https://docs.google.com/document/d/e/2PACX-1vSvM5gDlNvt7npYHhp_XfsJvuntUhq184By5xO_pA4b_gCWeXb6dM6ZxwN8rE6S4ghUsCj2VKR21oEP/pub";
	*/

	html = getHTML(url);

	// This token is the beggining reference of the characteres coordinates table in the HTML code
	html = html.substr(html.find("y-coordinate"));

	// Loop to extract the coordinates and the corresponding character from the HTML code until there is no more data to extract
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

		lettersMap[y][x] = letter; // Store the letter and coor diantes in the map

	} while (!html.empty());

	// Print the letters in the correct order based on their coordinates
	for (int yid = (int)lettersMap.size() - 1; yid >= 0 ; yid--)
	{
		for (int xid = 0 ; xid <= lettersMap[yid].size() - 1; xid++)
		{
			letter = lettersMap[yid][xid];

			if (letter != "")
				cout << letter;
			else
				cout << " ";
		}

		cout << endl;
	}

    return 0;
}
