#!/bin/bash

echo ""
echo "This script will automatically download"
echo "source files and compile 'math_solver' from"
echo "[https://github.com/eWillyo/ExpressionParser]."
echo ""
sleep 2
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

g++ -Wall -DNDEBUG -I./glm config.h constants.h error.h error.cpp functions.h functions.cpp operations.h operations.cpp types.h types.cpp parser.h parser.cpp main.cpp -o math_solver &> /dev/null

chmod +x math_solver
ln -s ExpressionParser/math_solver $HOME/ &>/dev/null

echo "Done!"
sleep 2
echo "Executing.."

./math_solver
