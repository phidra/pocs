#include <iostream>
#include <curl/curl.h>

using namespace std;

int main(void) {
    CURL* curl;
    CURLcode result;

    curl = curl_easy_init();
    if (!curl) {
        cerr << "Unable to init curl" << endl;
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    for (int i = 0; i < 10; ++i) {
        cout << "Requesting..." << endl;
        result = curl_easy_perform(curl);
        /* if(result != CURLE_OK) */
        /* { */
        /*     cerr << "curl_easy_perform() failed: " << curl_easy_strerror(result) << endl; */
        /* } */
    }
    curl_easy_cleanup(curl);
    return 0;
}
