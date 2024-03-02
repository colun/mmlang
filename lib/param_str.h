const string & paramStr(const string & name, const string & default_value = "") {
    std::map<std::string, std::string>::const_iterator it = mm$param$args.find(name);
    if(it!=mm$param$args.end()) {
        return it->second;
    }
    return default_value;
}
const string & paramStr(int i, const string & default_value = "") {
    if(i<mm$param$argsList.size()) {
        return mm$param$argsList[i];
    }
    return default_value;
}
