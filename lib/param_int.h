int paramInt(const string & name, int default_value) {
    std::map<std::string, std::string>::const_iterator it = mm$param$args.find(name);
    if(it!=mm$param$args.end()) {
        return atoi(it->second.c_str());
    }
    return default_value;
}
int paramInt(int i, int default_value) {
    if(i<mm$param$argsList.size()) {
        return atoi(mm$param$argsList[i].c_str());
    }
    return default_value;
}
