

scriptCall=$1
rootDir=$(pwd)
rm -rf Examples
mkdir Examples
mkdir Examples/Test1
mkdir Examples/Test1/Workone
mkdir Examples/Test2
mkdir Examples/Test3
mkdir Examples/Test3/WorkThree

cd Examples/Test3
touch test3f
cd WorkThree
touch three3f
cd "${rootDir}"

cd Examples/Test1
touch testone
cd Workone
touch testworkone
cd "${rootDir}"

mkdir Examples/Test5
mkdir Examples/Test5/WorkFive

cd Examples/Test5
touch test5f
cd WorkFive
touch five5f
cd "${rootDir}"

mkdir Examples/Test6
mkdir Examples/Test6/Dontworksix
mkdir Examples/Test7
mkdir Examples/Test7/Workseven
mkdir Examples/Test8
mkdir Examples/Test9
mkdir Examples/Test9/DontworkNine

mkdir Examples/Test33
mkdir Examples/Test33/Dontworkthree

cd Examples/Test33
touch testthree
cd Dontworkthree
touch donttestthree
cd "${rootDir}"


cd Examples/Test7
touch testseven
cd Workseven
touch testworkseven
cd "${rootDir}"


cd Examples/Test9
touch testnine
cd DontworkNine
touch donttestnine
cd "${rootDir}"


mkdir Examples/Test4
mkdir Examples/Test4/WorkFour


yellow()  { ansi 33 "$@"; }
red()   { ansi 31 "$@"; }
green()   { ansi 32 "$@"; }
purple()  { ansi 35 "$@"; }
ansi()    { echo -e "\033[0;${1}m${2}\033[0m"; }




Task1()
{
echo "Script should display a help message"
echo "=============================="
read -p "Press enter to continue " key
echo ""
echo ""
echo ""
sh ./$scriptCall -h
echo "$(yellow "${scriptCall} -h")"
echo ""
echo ""
echo ""
read -p "Did the script display a help message? (y/n): " condition
if [ "$condition" = "y" ]; then
  echo "Test 1: help function passed"
  echo ""
  echo ""
  echo ""
else
  echo "Test 1 Failed."
  exit 1
fi
}

Task2()
{
echo "Script should display a error message"
echo "=============================="
read -p "Press enter to continue " key
echo ""
echo ""
echo ""
sh ./$scriptCall
echo "$(yellow "${scriptCall}")"
echo ""
echo ""
echo ""
read -p "Did the script display a erro message? (No arguments) (y/n): " condition
if [ "$condition" = "y" ]; then
  echo "Test 2: no arguments test passed"
  echo ""
  echo ""
  echo ""
else
  echo "Test 2 Failed."
  exit 1
fi
}

Task3()
{
echo "Script should display a error message"
echo "=============================="
read -p "Press enter to continue " key
echo ""
echo ""
echo ""
sh ./$scriptCall -h -u
echo "$(yellow "${scriptCall} -h -u")"
echo ""
echo ""
echo ""
read -p "Did the script display a error message? (y/n): " condition
if [ "$condition" = "y" ]; then
  echo "Test 3: help with extra arguments passed"
  echo ""
  echo ""
  echo ""
else
  echo "Test Failed."
  exit 1
fi
}

Task4()
{
echo "Script should display a error message"
echo "=============================="
read -p "Press enter to continue " key
echo ""
echo ""
echo ""
sh ./$scriptCall -l -u
echo "$(yellow "${scriptCall} -l -u")"
echo ""
echo ""
echo ""
read -p "Did the script display a error message? (y/n): " condition
if [ "$condition" = "y" ]; then
  echo "Test 4: extra arguments (-l and -u together) passed"
  echo 
  echo
  echo
else
  echo "Test 4 Failed."
  echo 
  echo
  exit 1
fi
}


Task5()
{
echo "Script should uppercase the files in test3 folder and only that folder"
echo "=============================="
read -p "Press enter to continue " key
echo ""
echo ""
echo ""
sh ./$scriptCall -u Examples/Test3
echo "$(yellow "$scriptCall -u Examples/Test3")"
echo ""
echo ""
echo ""
read -p "Did the script work? (y/n): " condition
if [ "$condition" = "y" ]; then
  echo "Test 5: uppercase a directory passed without recursion"
  echo ""
  echo ""
  echo ""
else
  echo "Test 5 Failed."
  exit 1
fi
}

Task6()
{
echo "Script should uppercase the file in test5 folder and every file inside it"
echo "=============================="
read -p "Press enter to continue " key
echo ""
echo ""
echo ""
sh ./$scriptCall -u -r Examples/Test5
echo "$(yellow "$scriptCall -u -r Examples/Test5")"
echo ""
echo ""
echo ""
read -p "Did the script work? (y/n): " condition
if [ "$condition" = "y" ]; then
  echo "Test 6: uppercase with recursion passed"
  echo ""
  echo ""
  echo ""
else
  echo "Test 6 Failed."
  exit 1
fi
}

Task7()
{
echo "Script should substitute the files in test7 and everything inside it starting from test"
echo "=============================="
read -p "Press enter to continue " key

sh ./$scriptCall -r 's/test/SEDD/' Examples/Test1
echo "$(yellow "$scriptCall -r 's/test/SEDD/' Examples/Test1")"
echo ""
echo ""
echo ""
read -p "Did the script work? (y/n): " condition
if [ "$condition" = "y" ]; then
  echo "Test 7: sed with recursion passed"
  echo ""
  echo ""
  echo ""
else
  echo "Test 7 Failed."
  exit 1
fi
}

Task8()
{
echo "Script should substitute the files in test9 and only inside it"
echo "=============================="
read -p "Press enter to continue " key

sh ./$scriptCall 's/test/SEDD/' Examples/Test33
echo "$(yellow "$scriptCall 's/test/SEDD/' Examples/Test33")"
echo ""
echo ""
echo ""
read -p "Did the script work? (y/n): " condition
if [ "$condition" = "y" ]; then
  echo "Test 8: sed without recursion passed"
  echo ""
  echo ""
  echo ""
else
  echo "Test 8 Failed."
  exit 1
fi
}

Task9()
{
echo "Script should run sed command on multiple Test5 and Test7 folders in test9 and only inside it"
echo "=============================="
read -p "Press enter to continue " key

sh ./$scriptCall -r 's/test/SEDD/' Examples/Test9 Examples/Test7
echo "$(yellow "$scriptCall -r 's/test/SEDD/' Examples/Test9 Examples/Test7")"
echo ""
echo ""
echo ""
read -p "Did the script work? (y/n): " condition
if [ "$condition" = "y" ]; then
  echo "Test 9: sed with recursion with multiple files passed"
  echo ""
  echo ""
  echo ""
else
  echo "Test 9 Failed."
  exit 1
fi
}


# help command
Task1

# no arguments presented
Task2

# wrong arguments presented
Task3

# both -l and -u arguments given at once
Task4

# uppercase without  recursion
Task5

# uppercase with recursion
Task6

# sed command with recursion
Task7

# sed command without recursion
Task8

# sed command with recursion and multiple file names
Task9
