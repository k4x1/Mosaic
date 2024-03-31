#include "Downloader.h"



CDownloader::CDownloader()
{
}
CDownloader::~CDownloader()
{
    if (m_curl) {
        curl_easy_cleanup(m_curl);
    }
}

void CDownloader::Init()
{
	if (m_globalInit == false)
	{
		curl_global_init(CURL_GLOBAL_DEFAULT);
		m_globalInit = true;
	}
	m_curl = curl_easy_init();
}

bool CDownloader::Download(const char* _url, std::string& _outputStr)
{
    CURL* m_curl = curl_easy_init();
    if (m_curl)
    {
        CURLcode res;
        curl_easy_setopt(m_curl, CURLOPT_URL, _url);
        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, CDownloader::WriteData);
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &_outputStr);
        curl_easy_setopt(m_curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        //curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L); //uncomment for debug
        res = curl_easy_perform(m_curl);
        if (res != CURLE_OK)
        {
            std::cerr << " CURL error: " << res << "\n";
        }
        curl_easy_cleanup(m_curl);
        return res == CURLE_OK;
    }
    return false;
}
bool CDownloader::DownloadToFile(const char* _url, const char* _outputFile)
{
    //https://stackoverflow.com/questions/6645550/how-to-save-image-using-libcurl
    CURL* m_curl = curl_easy_init();
    if (m_curl)
    {
        FILE* fp;
        fopen_s(&fp, _outputFile, "wb");
        if (fp == NULL)
        {
            std::cerr << "File cannot be opened\n";
            return false;
        }

        CURLcode res;
        curl_easy_setopt(m_curl, CURLOPT_URL, _url);
        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, NULL); // Use the default write function
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, fp); // Set the file pointer as the write data
        curl_easy_setopt(m_curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        //curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L); // Uncomment for debug

        res = curl_easy_perform(m_curl);
        if (res != CURLE_OK)
        {
            std::cerr << "CURL error: " << res << "\n";
        }

        curl_easy_cleanup(m_curl);
        fclose(fp); // Close the file after the download
        return res == CURLE_OK;
    }
    return false;
}


size_t CDownloader::WriteData(void* _buffer, size_t _size, size_t _nmemb, void* _param)
{
	std::string& text = *static_cast<std::string*>(_param);
	size_t totalsize = _size * _nmemb;
	text.append(static_cast<char*>(_buffer), totalsize);
	return totalsize;
}
