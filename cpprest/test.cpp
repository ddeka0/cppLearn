#include <bits/stdc++.h>
#include <sys/time.h>
#include <locale>
#include <ctime>
#include "yaml-cpp/yaml.h"
#include "cpprest/json.h"
#include "cpprest/http_listener.h"
#include "cpprest/uri.h"
#include "cpprest/asyncrt_utils.h"
#include "cpprest/filestream.h"
#include "cpprest/containerstream.h"
#include "cpprest/producerconsumerstream.h"
#include <cpprest/http_client.h>
using namespace std;
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener; 
using namespace web::http;
using namespace web::http::client;

int main() {
	json::value a;
	a["x"] = web::json::value::array(1);
	a["x"].as_array()[0] = json::value::string("debashish");

	cout << a.serialize() << endl;



	utility::string_t address = U("test.com");
	http::uri uri = http::uri(address);

	http::uri_builder urib = http::uri_builder(uri)
							.append_path(U("nnrf-disc/v1/nf-instances"));

	std::map<std::string,json::value> nrfQueryOptions;

	nrfQueryOptions["target-nf-type"] = json::value::string("SMF");
	nrfQueryOptions["requester-nf-type"] = json::value::string("AMF");
	nrfQueryOptions["services"] = json::value::array(1);
	nrfQueryOptions["services"].as_array()[0]
								= json::value::string("nudm-uecm");


	json::value snssai;
	snssai["sst"] = json::value::number(5);
	snssai["sd"] = json::value::string("default-sd");

	nrfQueryOptions["snssais"] = web::json::value::array(1);
	nrfQueryOptions["snssais"].as_array()[0] = snssai;

	for(auto& [qname,qval] : nrfQueryOptions) {
		urib = urib.append_query(qname,qval);
	}

	cout << urib.to_string() << endl;
	
}