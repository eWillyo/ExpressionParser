echo "Installing necessary software.."

sudo apt update &> /dev/null
sudo apt install git g++ -y &> /dev/null

echo "Done!"
sleep 2
echo "Downloading source code.."

git clone https://github.com/eWillyo/ExpressionParser &> /dev/null

cd ExpressionParser

git clone https://github.com/g-truc/glm &> /dev/null

echo "Done!"
sleep 2
echo "Compiling.."

g++ -Wall -DNDEBUG -I ./glm constants.h operations.h operations.cpp types.h types.cpp main.cpp -o math_solver &> /dev/null

chmod +x math_solver

echo "Done!"
sleep 2
echo "Executing.."

./math_solver

