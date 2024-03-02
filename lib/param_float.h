double paramFloat(const string & name, double default_value) {
    std::map<std::string, std::string>::const_iterator it = mm$param$args.find(name);
    if(it!=mm$param$args.end()) {
        return atof(it->second.c_str());
    }
    return default_value;
}
double paramFloat(int i, double default_value) {
    if(i<mm$param$argsList.size()) {
        return atof(mm$param$argsList[i].c_str());
    }
    return default_value;
}
