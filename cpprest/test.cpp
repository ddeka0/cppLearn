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

/* some test code .......................... */
enum {
	REQUESTER_NF_TYPE   = 0,
	TARGET_NF_TYPE,
	SERVICE_NAMES,
	SNSSAIS,
	DNN,
};

map<std::string,int> queryIdMap = {
	{"target-nf-type",          TARGET_NF_TYPE},
	{"requester-nf-type",       REQUESTER_NF_TYPE},
	{"service-names",           SERVICE_NAMES},
	{"snssais",                 SNSSAIS},
	{"dnn",                     DNN}
};

std::string getQueryString(std::map<std::string,json::value>& queryOptions) {
	std::set<std::string> Set;
	for(auto &e : queryOptions) {
		switch(queryIdMap[e.first])
		{
			case REQUESTER_NF_TYPE:
			case TARGET_NF_TYPE:
			case DNN:
			{
				Set.insert(e.second.as_string());
			}
			break;
			case SERVICE_NAMES:
			{
				for(auto &x: e.second.as_array()) {
					Set.insert(x.as_string());
				}
			}
			break;
			case SNSSAIS:
			{
				for(auto &x: e.second.as_array()) {
					Set.insert(x.at("sd").as_string());
					Set.insert(std::to_string(x.at("sst").as_number().to_int32()));
				}
			}
			break;
			default:
				cerr << "Unknown query name type" << endl;
		}
	}
	std::string res = "";
	for(auto &e:Set) {
		res += e;
	}
	return res;
}






int main() {

	/* example query bulding for some dummy request for nrf */

	utility::string_t address = U("http://test.com/");
	http::uri uri = http::uri(address);

	http::uri_builder urib = http::uri_builder(uri)
							.append_path(U("nnrf-disc/v1/nf-instances"));

	std::map<std::string,json::value> nrfQueryOptions;

	nrfQueryOptions["target-nf-type"] = json::value::string("SMF");
	nrfQueryOptions["requester-nf-type"] = json::value::string("AMF");
	nrfQueryOptions["service-names"] = json::value::array(1);
	nrfQueryOptions["service-names"].as_array()[0]
								= json::value::string("nudm-uecm");

	json::value snssai;
	snssai["sst"] = json::value::number(1);
	snssai["sd"] = json::value::string("FFFFFF"); // dummy SD

	nrfQueryOptions["snssais"] = web::json::value::array(1);
	nrfQueryOptions["snssais"].as_array()[0] = snssai;

	nrfQueryOptions["dnn"] = json::value::string("test.com");

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
	cout <<"printing .. queries provided by the map ... " << endl<<endl;    
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
	cout << endl;


	/*someting extra ... not related to above */
	cout <<"printing .. snssais and service-names ... " << endl<<endl;    
	for(auto &e:nrfQueryOptions["snssais"].as_array()) {
		cout << e.at("sst").as_number().to_int32() <<" "<<e.at("sd").as_string() << endl;
	}

	for(auto &e:nrfQueryOptions["service-names"].as_array()) {
		cout << e.as_string() << endl;
	}
	cout << endl;

	cout <<"printing .. total query string ... " << endl;
	cout << getQueryString(nrfQueryOptions) << endl<<endl;



	/* some other test ............................... */
	cout <<"checking .. created hint is equal to query string or not ... " << endl<<endl;    
	std::multiset<std::string> Set;
	Set.insert(std::to_string(1)); // UE ID
	Set.insert(std::to_string(1));
	Set.insert("FFFFFF");
	Set.insert("AMF");
	Set.insert("SMF");
	Set.insert("nudm-uecm");
	Set.insert("test.com");
	std::string res = "";
	for(auto &e : Set) {
		res += e;
	}
	cout << ((res == getQueryString(nrfQueryOptions))?"query string matched":"query string does not match")<< endl;


	json::value A;
	json::value B;

	B["x"] = json::value::string("debashish");

	A["y"] = B;

	if(A.has_object_field(U("y"))) {
		cout << A.at(U("y")).at(U("x")).as_string() << endl;
	}

	std::cout <<"pplx thread test A ......................... "<<std::endl<<endl;




	// This is one way ....
	int cnt = 5;
	auto t = pplx::task<std::string>([&cnt]()
	{
		std::cout <<"Entry1 with TID : "<<std::this_thread::get_id()<<std::endl;
		// Do someting else here 
		while(cnt >= 0) {
			std::cout <<"loop .... "<<cnt--<<std::endl;
		}
		return std::string("Hello");
	})
	.then([](string x)
	{	
		std::cout <<"Entry2 with TID : "<<x<<" "<<std::this_thread::get_id()<<std::endl;
	});

	std::cout <<"Main Thread with TID : "<<std::this_thread::get_id()<<std::endl <<std::endl;

	std::cout <<"pplx thread test B ....................... "<< std::endl << endl;

	// This is one more way 
	pplx::task_from_result()
	.then([&cnt]()
	{	
		std::cout <<"Entry3 with TID : "<<std::this_thread::get_id()<< std::endl;
		// Do someting else here 
		while(cnt <= 5) {
			std::cout <<"loop .... "<<cnt++<<std::endl;
		}
		return std::string("Hello");		
	})
	.then([](string x)
	{	
		std::cout <<"Entry4 with TID : "<<x<<" "<<std::this_thread::get_id()<<std::endl;
	})
	.wait(); // please wait main thread, for these chain to complete


	t.wait(); // wait for 1 to complete now 
}
