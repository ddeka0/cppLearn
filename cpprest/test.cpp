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

	/* example query bulding for some dummy request for nrf */

	utility::string_t address = U("http://test.com/");
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
	snssai["sst"] = json::value::number(1);
	snssai["sd"] = json::value::string("FFFFFF"); // dummy SD

	nrfQueryOptions["snssais"] = web::json::value::array(1);
	nrfQueryOptions["snssais"].as_array()[0] = snssai;

	for(auto& [qname,qval] : nrfQueryOptions) {
		urib = urib.append_query(qname,qval);
	}

	cout <<"uri_builder => "<<urib.to_string() << endl << endl;
	uri = urib.to_uri();
	cout <<"uri => "<<uri.to_string() << endl<<endl;	// prints the same 


	/*simulate server in the same file */

    auto query =  uri::split_query(uri::decode(uri.query()));
    /*  before processing any query ............................ 
    *   for exmaple I want to get the query of requester-nf-type
    *   I know it is a string, therefore parse it as string from val
    *   similarly,
    *   services is an array of strings ( from standard )
    *   therefore first convert it to an array class and then convert it to
    *   string using as_string()
    * 
    *   These loop need not run beforehand, access each query whenever required
    *   and access accordingly
    */
    for(auto & e : query) {
        json::value val = json::value::parse(e.second);
        if(e.first == "requester-nf-type" or e.first == "target-nf-type") {
            std::string x = val.as_string();
            cout <<e.first<<" "<<x << endl;
        }else if(e.first == "snssais") {
            int sst =  val.as_array()[0].at("sst").as_number().to_int32();
            string sd = val.as_array()[0].at("sd").as_string();

            cout << e.first <<" sd = "<< sd <<" sst = "<<sst<< endl;
        }else if(e.first == "services") {
            cout <<e.first<<" "<<val.as_array()[0].as_string() <<endl;
        }
    }



}