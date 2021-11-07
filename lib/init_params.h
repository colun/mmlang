std::map<std::string, std::string> mm$param$args;
void mm$initParams(int argc, const char * * argv) {
    for(int i=2; i<argc; i+=2) {
        mm$param$args[argv[i-1]] = argv[i];
    }
}
void initParams(const string & pythonDictString) {
    const char * s = pythonDictString.c_str();
    std::string value;
    std::string key;
    if(*s=='{') ++s;
    const char * start = s;
    while(*s) {
        if(*s=='\'') {
            ++s;
            start = s;
            while(*s) {
                if(*s=='\'') {
                    value.assign(start, s);
                    start = NULL;
                    ++s;
                    break;
                }
                ++s;
            }
        }
        else if(*s==':') {
            if(start) value.assign(start, s);
            ++s;
            while(*s==' ') ++s;
            key = value;
            start = s;
        }
        else if(*s==',' || *s=='}') {
            if(start) value.assign(start, s);
            ++s;
            while(*s==' ') ++s;
            if(mm$param$args.find(key)==mm$param$args.end()) {
                mm$param$args[key] = value;
            }
            key.resize(0);
            value.resize(0);
        }
        else {
            ++s;
        }
    }
}
