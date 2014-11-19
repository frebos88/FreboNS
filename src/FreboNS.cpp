#include "Server.h"
#include "Config.h"
#include "Database.h"

#include "tld-canon.h"
#include "tld.h"

int main(int argc, char *argv[])
{
	tldnode* tree = readTldTree(tldString);

	std::cout << "Domain: " << getRegisteredDomain( (char*)"www.a.longer.domain.name.with.subdomains.example.co.uk", tree ) << std::endl;

        freeTldTree(tree);

	Config conf;
	if (!conf.load()) std::cout << "Could not load ini file" << std::endl;
	Db = new Database( conf.get("mysql", "host"), conf.get("mysql", "user"), conf.get("mysql", "password"), conf.get("mysql", "database") );
	
	if( !Db->Valid() ) {
		std::cout << "Database is not valid!" << std::endl;
		return 1;
	}
	
	std::cout << "-------------------------------------------------" << std::endl;
	std::cout << "FreboNS starting..." << std::endl;
	std::cout << "-------------------------------------------------" << std::endl << std::endl;

	bool output = false;
	::std::string outputTemp = conf.get("misc", "show_output");
	if (outputTemp == "1" || outputTemp == "true")
		output = true;
	
	Server s;
	s.start(output);
	
	std::cout << "Server started successfully." << std::endl;
	
	while( s.getPacket() )
	{
		/* */
	}
	
	s.stop();
	Db->Close();
	
	return 0;
}
