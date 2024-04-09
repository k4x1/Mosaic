#define NOMINMAX
#include "Downloader.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <mutex>
#include <memory>
#include "FileImages.h"
#include "Grid.h"
#include <thread>
#include "Button.h"
#include "ThreadPool.h"
/*auto downloadImage = [](const std::string& _url, const std::string& _filePath, CDownloader& _downloader) -> int {
    std::ifstream file(_filePath);
    if (!file.good()) {
        if (_downloader.DownloadToFile(_url.c_str(), _filePath.c_str())) {
            std::cout << "image download success: " << _url << std::endl;
        }
        else {
            std::cout << "image download FAILED: " << _url << std::endl;
        }
    }
    return 0;
};
*/

// Function to split URLs from a string
std::vector<std::string> splitUrls(const std::string& _data) {
    std::vector<std::string> urls;
    size_t pos = 0;
    size_t oldPos = 0;
    while ((pos = _data.find('\n', oldPos)) != std::string::npos) {
        urls.push_back(_data.substr(oldPos, pos - oldPos));
        oldPos = pos + 1;
    }
    return urls;
}

// Function to download images asynchronously
int downloadImage(const std::string& _url, const std::string& _filePath, CDownloader& _downloader) {
    std::ifstream file(_filePath);
    if (!file.good()) {
        if (_downloader.DownloadToFile(_url.c_str(), _filePath.c_str())) {
            std::cout << "image download success: " << _url << std::endl;

        }
        else {
            std::cout << "image download FAILED: " << _url << std::endl;
        }
    }
    return 0;
}

int main() {
    //button
   
    int currentPage = 0;
    sf::Font font;
    if (!font.loadFromFile("Avilock.ttf")) {
        // Handle error
        std::cout << "font failed";
    }
    //create grid here
    Grid grid;

    //buttons
    Button pageRight(sf::Vector2f(250, 100), sf::Vector2f(100, 50), font, "Page right", sf::Color::White, sf::Color::Green, sf::Color::Red,PAGE_RIGHT);
    Button pageLeft(sf::Vector2f(100, 100), sf::Vector2f(100, 50), font, "Page left", sf::Color::White, sf::Color::Green, sf::Color::Red, PAGE_LEFT);    
    Button screenshotButton(sf::Vector2f(150, 200), sf::Vector2f(150, 50), font, "     Screenshot", sf::Color::White, sf::Color::Green, sf::Color::Red, SCREENSHOT);
    
        //



    std::chrono::steady_clock::time_point startTime;


    std::vector<sf::Texture> imageTextures;
    std::vector<std::string> filePaths;
    const int windowSize = 900;
   
    bool fileSizeIsSmall = true;

    {
        sf::RenderWindow settingsSelectWindow(sf::VideoMode(windowSize, windowSize), "select settings");
        Button zoomIn(sf::Vector2f(600, 200), sf::Vector2f(100, 50), font, "Zoom in", sf::Color::White, sf::Color::Green, sf::Color::Red, ZOOM_IN);
        Button zoomOut(sf::Vector2f(200, 200), sf::Vector2f(100, 50), font, "Zoom out", sf::Color::White, sf::Color::Green, sf::Color::Red, ZOOM_OUT);
        Button done(sf::Vector2f(350, 400), sf::Vector2f(200, 50), font, "Done", sf::Color::White, sf::Color::Green, sf::Color::Red, DONE);
        Button fileSize(sf::Vector2f(350, 600), sf::Vector2f(200, 50), font, "file: ", sf::Color::White, sf::Color::Green, sf::Color::Red, FILE_SIZE);
        
        sf::Text sizeDisplay = sf::Text("Grid Dimensions: ", font, 24);
        sizeDisplay.setPosition(360,200); 
   
        while (settingsSelectWindow.isOpen()) {
            sf::Event winEvent;
      
            while (settingsSelectWindow.pollEvent(winEvent)) {
                if (winEvent.type == sf::Event::Closed || done.update(settingsSelectWindow.mapPixelToCoords(sf::Mouse::getPosition(settingsSelectWindow)), currentPage, grid)) {
       
                    settingsSelectWindow.close();
                }
                sizeDisplay.setString("Grid Dimensions: "+std::to_string(grid.m_gridSize));
                settingsSelectWindow.clear();
                zoomIn.update(settingsSelectWindow.mapPixelToCoords(sf::Mouse::getPosition(settingsSelectWindow)), currentPage, grid);
                zoomOut.update(settingsSelectWindow.mapPixelToCoords(sf::Mouse::getPosition(settingsSelectWindow)), currentPage, grid);
            
                fileSize.update(settingsSelectWindow.mapPixelToCoords(sf::Mouse::getPosition(settingsSelectWindow)), currentPage, grid, &fileSizeIsSmall);
                done.draw(settingsSelectWindow);
                settingsSelectWindow.draw(sizeDisplay);
                zoomIn.draw(settingsSelectWindow);
                zoomOut.draw(settingsSelectWindow);
                
                fileSize.draw(settingsSelectWindow);
                std::string fileText = fileSizeIsSmall ? "img small" : "img large";
                fileSize.m_text.setString("file: "+ fileText);
                settingsSelectWindow.display();
            }
        }
    }
    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "GD2P03 Assignment 1");
    sf::RenderWindow buttonWindow(sf::VideoMode(windowSize / 2, windowSize / 2), "Buttons");

    CDownloader downloader;
    downloader.Init();
    ThreadPool pool(std::thread::hardware_concurrency());

    auto fileToDownload = fileSizeIsSmall ? "https://raw.githubusercontent.com/MDS-HugoA/TechLev/main/ImgListSmall.txt" : "https://raw.githubusercontent.com/MDS-HugoA/TechLev/main/ImgListLarge.txt";
    std::string data;
    if (!downloader.Download(fileToDownload, data)) {
        std::cout << "data failed to download" << std::endl;
        return 1; // Exit if download fails
    }
    auto downloadImage = [](const std::string& _url, const std::string& _filePath, CDownloader& _downloader) -> int {
    std::ifstream file(_filePath);
    if (!file.good()) {
        if (_downloader.DownloadToFile(_url.c_str(), _filePath.c_str())) {
            std::cout << "image download success: " << _url << std::endl;
        }
        else {
            std::cout << "image download FAILED: " << _url << std::endl;
        }
    }
    return 0;
};

    std::vector<std::string> urls = splitUrls(data);
    std::vector<std::future<int>> futures;
    std::mutex countMutex;
    int count = 0;
    startTime = std::chrono::steady_clock::now();
    std::vector<std::future<int>> downloadFutures;
    for (const auto& url : urls) {
        std::string filePath = "Images/" + url.substr(url.find_last_of('/') + 1);
        filePaths.push_back(filePath);
        downloadFutures.push_back(pool.enqueue(downloadImage, url, filePath, std::ref(downloader)));
    }
    for (auto& future : downloadFutures) {
        future.wait();
    }
    // downloads have to go first 

    // Initialize grid

    int imageCount = urls.size();
    int percentageDone = 0;
    grid.InitGrid(imageCount);
    imageTextures.resize(imageCount);


    std::vector<std::future<void>> loadFutures;
    for (int i = 0; i < imageCount; i++) {
        loadFutures.push_back(pool.enqueue([&, i]() {
            if (!imageTextures[i].loadFromFile(filePaths[i])) {
                std::cout << "Failed to load image: " << filePaths[i] << std::endl;
            }
            else {
                grid.setTileTextures(&imageTextures[i]);
                percentageDone++;
                std::cout << "image set: " << percentageDone << "/" << imageCount << std::endl;
            }
            }));
    }

    
    int txtCount = 0;
    auto endTime = std::chrono::steady_clock::now(); // End the timer
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    std::cout << "Total time taken for program to load:  " << elapsedTime << " milliseconds" << std::endl;

    // Main loop
    while (window.isOpen() || buttonWindow.isOpen()) {
        sf::Event winEvent;
        while (window.pollEvent(winEvent)) {
            if (winEvent.type == sf::Event::Closed) {
              
                window.close();
            }
        }
        while (buttonWindow.pollEvent(winEvent)) {
            if (winEvent.type == sf::Event::Closed) {
                buttonWindow.close();
            }
        }
        window.clear();
        for (auto& row : grid.m_grid[currentPage]) {
            for (auto& tile : row) {
      
                window.draw(tile.m_image);
            }
        }
        window.display();
        
        pageRight.update(buttonWindow.mapPixelToCoords(sf::Mouse::getPosition(buttonWindow)), currentPage, grid);
         pageLeft.update(buttonWindow.mapPixelToCoords(sf::Mouse::getPosition(buttonWindow)), currentPage, grid); 
         screenshotButton.m_window = &window;
         screenshotButton.update(buttonWindow.mapPixelToCoords(sf::Mouse::getPosition(buttonWindow)), currentPage, grid);
       
        buttonWindow.clear();
        screenshotButton.draw(buttonWindow);
        pageRight.draw(buttonWindow);
        pageLeft.draw(buttonWindow);
       
        buttonWindow.display();
    }

    curl_global_cleanup();
    return 0;
}
