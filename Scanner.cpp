#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#ifdef __linux__
const std::string command = "iwlist scan";
#elif defined(__APPLE__)
const std::string command = "/System/Library/PrivateFrameworks/Apple80211.framework/Versions/Current/Resources/airport -s";
#elif defined(_WIN32) || defined(_WIN64)
const std::string command = "netsh wlan show networks mode=Bssid";
#else
#error "Unsupported platform"
#endif

struct WifiNetwork {
    std::string ssid;
    std::string bssid;
    std::string essid;
    std::string signalLevel;
    std::string frequency;
    
    // Add more fields as needed
};

std::vector<WifiNetwork> parseWifiNetworks(const std::string& output) {
    std::vector<WifiNetwork> networks;
    std::istringstream stream(output);

    std::string line;
    // Skip the header line
    std::getline(stream, line);

    while (std::getline(stream, line)) {
        std::istringstream lineStream(line);
        std::string ssid, bssid, essid, signalLevel, frequency;

        lineStream >> ssid;
        lineStream >> bssid;
        lineStream >> essid;
        lineStream >> signalLevel;
        lineStream >> frequency;

        // Create a WifiNetwork object and populate its fields
        WifiNetwork network;
        network.ssid = ssid;
        network.bssid = bssid;
        network.essid = essid;
        network.signalLevel = signalLevel;
        network.frequency = frequency;

        networks.push_back(network);
    }

    return networks;
}

int main() {
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Error: Failed to execute command.\n";
        return 1;
    }

    char buffer[128];
    std::string output;
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        output += buffer;
    }

    pclose(pipe);

    // Parse the Wi-Fi network information
    std::vector<WifiNetwork> networks = parseWifiNetworks(output);

    // Print the Wi-Fi network details
    for (const WifiNetwork& network : networks) {
        std::cout << "SSID: " << network.ssid << std::endl;
        std::cout << "BSSID: " << network.bssid << std::endl;
        std::cout << "ESSID: " << network.essid << std::endl;
        std::cout << "Signal Level: " << network.signalLevel << std::endl;
        std::cout << "Frequency: " << network.frequency << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
