inline double logit(double p) {
    return std::log(p) - std::log(1-p);
}
