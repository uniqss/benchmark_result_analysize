#include "simdjson.h"
using namespace simdjson; // optional

#include "stl.h"

#include "pjson.inl"

#include <stdint.h>

#define PJE(element) cout << #element << ":" << element << " type:" << element.type() << endl;

int64_t trim_usms(const string& s)
{
	if (s.find("us") != string::npos)
	{
		string avg = s.substr(0, s.size() - 2);
		auto f = atof(avg.c_str());
		return (int64_t)f;
	} else if (s.find("ms") != string::npos) {
		string avg = s.substr(0, s.size() - 2);
		auto f = atof(avg.c_str());
		f *= 1000;
		return (int64_t)f;
	} else if (s.find("s") != string::npos) {
		string avg = s.substr(0, s.size() - 2);
		auto f = atof(avg.c_str());
		f *= 1000000;
		return (int64_t)f;
	}
	return -1;
}

void trim_result(const std::vector<std::pair<std::string, std::vector<std::vector<int64_t>>>>& r) {
	int i = 0;
	for(const auto& p : r) {
		cout << i << ":" << p.first << endl;
	}
}

void help()
{
}

int main(int argc, const char** argv)
{
	string fullpath = "./results.2020-04-05-20-04-51-269.json";
	string operation = "fortune";

	if (argc > 1) {
		fullpath = argv[1];
	}
	if (argc > 2) {
		operation = argv[2];
	}

	dom::parser parser;
	auto [doc, err] = parser.load(fullpath); // load and parse a file
	//auto doc = parser.load(fullpath.c_str()); // load and parse a file
	if(err){
		cout << "load failed file:" << fullpath << endl;
		return -1;
	}
	auto rawData = doc["rawData"];
	auto op = rawData[operation.c_str()];
	std::vector<std::pair<string, vector<vector<int64_t>>>> lll;
	for (dom::key_value_pair kv : dom::object(op)) {
		auto k = kv.key;
		//cout << k << endl;
		auto v = kv.value;
		bool bfailed = false;
#define CMPBRK(e) if ((e)) { bfailed = true; break;}
		vector<vector<int64_t>> ll;
		for (auto child : dom::array(v)) {
			//PJE(child);
			auto [latencyAvg, e1] = child["latencyAvg"];
			CMPBRK(e1);
			//PJE(latencyAvg);
			auto [latencyMax, e2] = child["latencyMax"];
			CMPBRK(e2);
			//PJE(latencyMax);
			auto [latencyStdev, e3] = child["latencyStdev"];
			CMPBRK(e3);
			//PJE(latencyStdev);
			auto [totalRequests, e4] = child["totalRequests"];
			CMPBRK(e4);
			//PJE(totalRequests);
			auto [startTime, e5] = child["startTime"];
			CMPBRK(e5);
			//PJE(startTime);
			auto [endTime, e6] = child["endTime"];
			CMPBRK(e6);
			//PJE(endTime);
			vector<int64_t> l;
			l.push_back(trim_usms((string)latencyAvg));
			l.push_back(trim_usms((string)latencyMax));
			l.push_back(trim_usms((string)latencyStdev));
			l.push_back(totalRequests);
			l.push_back(startTime);
			l.push_back(endTime);
			ll.push_back(l);
		}
		if (bfailed){
			continue;
		}
		lll.push_back(std::make_pair((string)k, ll));
	}

	//auto hapi = fortune["hapi-postgres"];
	//cout << hapi << endl;
	//PJE(hapi);

	std::sort(lll.begin(), lll.end(), [](const std::pair<string, std::vector<std::vector<int64_t>>>& lhs, const std::pair<string, std::vector<std::vector<int64_t>>>& rhs){
			auto req_l = 0;auto req_r = 0;
			auto tm_l = 0; auto tm_r = 0;
			std::for_each(lhs.second.begin(), lhs.second.end(), [&](auto& vi){
					req_l += vi[3];
					tm_l += vi[5] - vi[4];
					});
			std::for_each(rhs.second.begin(), rhs.second.end(), [&](auto& vi){
					req_r += vi[3];
					tm_r += vi[5] - vi[4];
					});
			auto avg_l = req_l / tm_l;
			auto avg_r = req_r / tm_r;
			return (bool)(avg_l > avg_r);
			});

	trim_result(lll);
	return 0;
}

