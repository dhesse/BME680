#include <string>
#include <list>

class JFieldBase
{
private:
    JFieldBase *next;

protected:
    void format_next(char *buf)
    {
        if (next)
        {
            buf[0] = ',';
            next->format(buf + 1);
        }
        else 
        {
            sprintf(buf, "}");
        }
    }

public:
    JFieldBase(JFieldBase *next) : next(next){};
    virtual void format(char *buf) = 0;
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
    void format(char *buf);
};

template <>
void JField<int>::format(char *buf)
{
    format_next(buf + sprintf(buf, "\"%s\": %i", name, value));
}

template <>
void JField<unsigned long>::format(char *buf)
{
    format_next(buf + sprintf(buf, "\"%s\": %lu", name, value));
}

template <>
void JField<float>::format(char *buf)
{
    format_next(buf + sprintf(buf, "\"%s\": %.2f", name, value));
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
    void format(char* buf){
        buf[0] = '{';
        current->format(buf+1);
    }
};