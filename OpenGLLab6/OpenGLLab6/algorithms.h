#include <string>

namespace Algorithms
{
    class Algorithm
    {
    public:
        Algorithm(std::string name);
        const std::string getName() const;
        virtual void apply() = 0;

    private:
        const std::string _name;
    };

    class MidPointAlgorithm final : public Algorithm
    {
    public:
        MidPointAlgorithm();
        virtual void apply() override;
    };

    class AntiAliasingAlgorithm final : public Algorithm
    {
    public:
        AntiAliasingAlgorithm();
        virtual void apply() override;
    };
}