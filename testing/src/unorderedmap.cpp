#include <unordered_map>
#include <iostream>
#include <set>
#include <algorithm>
#include <vector>
#include <iterator>

class StrWrapper {
    public:
        StrWrapper(const char* c) : str(c) {}
        StrWrapper& operator=(const StrWrapper& rhs) { str = rhs.str; return *this; }
        std::string ShowMe() { return str; }

    private:
        std::string str;
};

class Iterator {
    using bucket_iterator = std::unordered_multimap<std::string, StrWrapper*>::local_iterator;

    public:
        Iterator(bucket_iterator, bucket_iterator);
        Iterator(const Iterator&);
        Iterator& operator=(const Iterator&);

        StrWrapper& operator*() { return *str; }
        StrWrapper* operator->() { return &**this; }
        Iterator& operator++();
        Iterator operator++(int);
        bool operator==(const Iterator&);
        bool operator!=(const Iterator&);

    private:
        bucket_iterator it;
        bucket_iterator end;
        StrWrapper* str;
        
};

Iterator::Iterator(bucket_iterator b, bucket_iterator e) : it(b), end(e) {
    if (it != end)
        str = it->second;

}

Iterator::Iterator(const Iterator& rhs) : it(rhs.it), end(rhs.end) {
    if (it != end)
        str = it->second;

}

Iterator& Iterator::operator=(const Iterator& rhs) {
    it = rhs.it;
    end = rhs.end;
    if (it != end)
        str = rhs.str;
    return *this;
}

Iterator& Iterator::operator++() {
    it++;
    if (it != end)
        str = it->second;
    return *this;
}

Iterator Iterator::operator++(int) {
    bucket_iterator it_old = it++;
    if (it != end)
        str = it->second;
    return Iterator(it_old, end);
}

bool Iterator::operator==(const Iterator& rhs) {
    return (it == rhs.it);
}

bool Iterator::operator!=(const Iterator& rhs) {
    return !(*this == rhs);
}

Iterator BktBegin(std::unordered_multimap<std::string,StrWrapper*>& um,
                const std::string& str) {
    auto bkt = um.bucket(str);
    return {um.begin(bkt), um.end(bkt)};
}

Iterator BktEnd(std::unordered_multimap<std::string,StrWrapper*>& um,
                const std::string& str) {
    auto bkt = um.bucket(str);
    return {um.end(bkt), um.end(bkt)};
}

int main() {

    StrWrapper* me = new StrWrapper{"Middle East"};
    StrWrapper* a = new StrWrapper{"Africa"};
    StrWrapper* sa = new StrWrapper{"South America"};
    StrWrapper* na = new StrWrapper{"North America"};

  std::unordered_multimap<std::string,StrWrapper*> myumm = {
    {"John",me},
    {"John",a},
    {"John",sa},
    {"John",na},
    {"Adam",me},
    {"Adam",a},
    {"Adam",sa},
    {"Adam",na},
    {"Bill",me},
    {"Bill",a},
    {"Bill",sa},
    {"Bill",na}
  };

  for (auto& x: myumm) {
    std::cout << "Element [" << x.first << ":" << x.second << "]";
    std::cout << " is in bucket #" << myumm.bucket (x.first) << std::endl;
  }

  auto bkt = myumm.equal_range("John");
  for (auto it = bkt.first; it != bkt.second; ++it)
    std::cout << it->first << " " << it->second << "\n";

  //    for (auto& x: myumm) {
  //    std::cout << "Element [" << x.first << ":" << x.second << "]";
  //    std::cout << " is in bucket #" << myumm.bucket (x.first) << std::endl;
  //    }
    std::cout << "=-=- Iterating through the custom iterator class =-=-\n";
    std::cout << "Bucket \"John\"\n";
    int cnt = 0;
    for (auto it = BktBegin(myumm, "John"); it != BktEnd(myumm, "John"); ++it)
        std::cout << it->ShowMe() << "\n"; 
    

  return 0;
}