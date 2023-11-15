#include <iostream>
#include <string>
#include <vector>
#include <utility>

/// HTML Builder
/// We want to build HTML structure using a special domain specific language
/// that is similar to the HTML source files:  P{ P { IMG {} } }

namespace Html {

    /// Generic tag.
    class Tag {
        public:
            std::string name;           ///< The tags name (P, IMG, etc.)
            std::string text;           ///< The text inside the tag.
            std::vector<Tag> children;  ///< The other tags inside this one.
                                        ///< Note that all the subclasses have no member variables,
                                        ///< we do not have to fear the slicing. 
            std::vector<std::pair<std::string, std::string>> attr;    ///< The attributes.

        protected:
            Tag(const std::string &name, const std::string &text) : name{ name }, text{ text } {
			      }
			
            Tag(const std::string &name, const std::vector<Tag> &children) : name{ name }, children{ children } {    
            }
	    
    };  // class Tag

    struct P: public Tag {
        P(const std::string &text) : Tag{ "P", text } {}
	      P(std::initializer_list<Tag> children) : Tag{ "P", children } {}
    };

    struct IMG :public Tag {
        IMG(const std::string &url) : Tag{ "IMG", "" } {
            attr.emplace_back("src", url);
        }
    };

    // It is in the Html namespace but ADL will kick in
    std::ostream& operator<<(std::ostream &os, const Tag &tag) {
        os << "<" << tag.name << ">";
        for (const auto &x : tag.children) {
            os << x;
        }
        os << "</" << tag.name << ">";
        return os;
    }
    
}  // namespace Html

int main() {

    {
        // We can spot here a copy elision :)
        // outputs just one P: <P><IMG></IMG><IMG></IMG></P>

        auto v = Html::P{
            Html::P{
                Html::IMG("http://nintedo.com/pokedex/picachu.png"),
                Html::IMG("http://nintedo.com/pokedex/mewtwo.png"),
            }
        };

        std::cout << v;
    }

    std::cout << "\n-------------------------\n";

    {
        // we enforce initializer list
        // outputs the two Ps: <P><P><IMG></IMG><IMG></IMG></P></P>

        auto v = Html::P{
            {                              //<--- this was added to make initializer list
                Html::P{
                    Html::IMG("http://nintedo.com/pokedex/picachu.png"),
                    Html::IMG("http://nintedo.com/pokedex/mewtwo.png"),
                }
            }                              //<--- this was added
        };

        std::cout << v;
    }

    return 0;
}
