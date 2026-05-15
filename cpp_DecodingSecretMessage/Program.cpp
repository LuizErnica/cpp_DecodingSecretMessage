#include <iostream>
#include <string>
#include <vector>
#include <regex>
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

string removeTags(const string& input)
{
    return regex_replace(input, regex("<[^>]*>"), "");
}

int main()
{
    string url =
        "https://docs.google.com/document/d/e/"
        "2PACX-1vTMOmshQe8YvaRXi6gEPKKlsC6UpFJSMAk4mQjLm_u1gmHdVVTaeh7nBNFBRlui0sTZ-snGwZM4DBCT/pub";

    string html = downloadHTML(url);

    regex tableRegex("<table[^>]*>(.*?)</table>",
        regex_constants::icase |
        regex_constants::dotall);

    regex rowRegex("<tr[^>]*>(.*?)</tr>",
        regex_constants::icase |
        regex_constants::dotall);

    regex cellRegex("<t[dh][^>]*>(.*?)</t[dh]>",
        regex_constants::icase |
        regex_constants::dotall);

    smatch tableMatch;

    if (regex_search(html, tableMatch, tableRegex))
    {
        string tableHTML = tableMatch[1];

        auto rowBegin =
            sregex_iterator(tableHTML.begin(),
                tableHTML.end(),
                rowRegex);

        auto rowEnd = sregex_iterator();

        for (auto rowIt = rowBegin; rowIt != rowEnd; ++rowIt)
        {
            string rowHTML = (*rowIt)[1];

            auto cellBegin =
                sregex_iterator(rowHTML.begin(),
                    rowHTML.end(),
                    cellRegex);

            auto cellEnd = sregex_iterator();

            for (auto cellIt = cellBegin;
                cellIt != cellEnd;
                ++cellIt)
            {
                string cell = (*cellIt)[1];

                cell = removeTags(cell);

                cout << cell << " | ";
            }

            cout << endl;
        }
    }
    else
    {
        cout << "Tabela nao encontrada." << endl;
    }

    return 0;
}
