#include <string>

namespace Algorithms
{
    class Algorithm
    {
    public:
        explicit Algorithm(const std::string &name);
        std::string getName() const;
        virtual void apply() = 0;
        virtual ~Algorithm();
    private:
        const std::string _name;
    };

    class MidPointAlgorithm final : public Algorithm
    {
    public:
        MidPointAlgorithm();
        void apply() override;
    };

    class AntiAliasingAlgorithm final : public Algorithm
    {
    public:
        AntiAliasingAlgorithm();
        void apply() override;
    };
}