
struct ConnexionParameters
{
  std::string server;
  uint16_t port;
};

struct Address {
};


using ConnexionParametersDB = std::map<std::string, ConnexionParameters>;
ConnexionParametersDB& getDB() {
  static std::map<std::string, ConnexionParameters> 
}

boost::optional<ConnexionParameters> find(Address const& a) {
}


int main() {
}