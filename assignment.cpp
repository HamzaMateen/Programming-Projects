#include <iostream>
#include <iomanip>

float calcGradePoints (char marks, int credits) {
  float gradePoints;

  if (marks >= 80) 
    gradePoints = 4 * credits;  
  else if (marks >= 75) 
    gradePoints = 3.5 * credits;
  else if (marks >= 70)
    gradePoints = 3 * credits;
  else if (marks >= 65)
    gradePoints = 2.5 * credits;
  else if (marks >= 60)
    gradePoints = 2 * credits;
  else if (marks >= 55)
    gradePoints = 1.5 * credits;
  else if (marks >= 50)
    gradePoints = credits;
  else gradePoints = 0;


  return gradePoints;
}

float calculateGPA () {
  const int TOTAL_MARKS = 100;
  const int SUBJECTS_COUNT = 9;

  const char* subjectArr[] = {"Arabic-1", "Functional English", "Applied Calculus", "Engineering Physics", "Islamic Studies", "Fundamentals of Programming", "Workshop Practice", "Engineering Physics Lab", "Fundamentals of Programming Lab"};

  const int creditHours[] = {2, 3, 3, 3, 3, 1, 1, 1, 1};

  int marks;
  float totalGradePoints;;

  for (int i = 0; i < SUBJECTS_COUNT; i++) {
    std::cout << "Enter your marks for " << subjectArr[i] << " : ";
    std::cin >> marks;

    float currentGradePoints = calcGradePoints (marks, creditHours[i]);

    totalGradePoints += currentGradePoints;
  }

  return totalGradePoints / 18;
}
int main() {

  std::cout << "******************WELCOME TO SEMESTER GPA CALCULATOR!******************\n\n" << std::endl;

  float sgpa = calculateGPA();

  std::cout << "____________________________________________" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "\nSemester GPA (SGPA) : " << sgpa << std::endl;
  std::cout << "Semester Cumulative Percentage : " << (sgpa / 4.25) * 100 << std::endl;
  std::cout << "____________________________________________" << std::endl;

  std::cout << "\n\n******************THANKS FOR USING SGPA CALCULATOR******************" << std::endl;


  return 0;
}