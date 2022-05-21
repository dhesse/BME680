#include <string>
#include <list>

class JFieldBase
{
private:
    JFieldBase *next;

protected:
    int format_next(char *buf)
    {
        int res = 1;
        if (next)
        {
            buf[0] = ',';
            res += next->format(buf + 1);
        }
        else 
        {
            sprintf(buf, "}");
        }
        return res;
    }

public:
    JFieldBase(JFieldBase *next) : next(next){};
    virtual int format(char *buf) = 0;
    JFieldBase* get_next() { return next; };
    virtual ~JFieldBase() {
        delete next;
    };
};

template <typename T>
class JField : public JFieldBase
{
private:
    const char *name;
    T value;

public:
    JField(const char *name, T value, JFieldBase *next) : JFieldBase(next), name(name), value(value){};
    int format(char *buf);
};

template <>
int JField<int>::format(char *buf)
{
    int size = sprintf(buf, "\"%s\": %i", name, value);
    return format_next(buf + size) + size;
}

template <>
int JField<unsigned long>::format(char *buf)
{
    int size = sprintf(buf, "\"%s\": %lu", name, value);
    return format_next(buf + size) + size;
}

template <>
int JField<float>::format(char *buf)
{
    int size = sprintf(buf, "\"%s\": %.2f", name, value);
    return format_next(buf + size) + size;
}

class JSON {
    private:
    JFieldBase* current;
    public:
    JSON() : current(NULL) {};
    ~JSON() {
        delete current;
    }
    template<typename T> void push(const char* name, T value) {
        current = new JField<T>(name, value, current);
    }
    int format(char* buf){
        buf[0] = '{';
        return 1 + current->format(buf+1);
    }
};