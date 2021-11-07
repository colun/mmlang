double paramInt(const string & name, int default_value) {
    std::map<std::string, std::string>::const_iterator it = mm$param$args.find(name);
    if(it!=mm$param$args.end()) {
        return atoi(it->second.c_str());
    }
    return default_value;
}
