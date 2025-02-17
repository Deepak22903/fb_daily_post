#include <bits/stdc++.h>
#include <iostream>
using namespace std;

string caption_randomizer(vector<string> captions) {
  int lower = 0;
  int upper = captions.size() - 1;
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> distr(lower, upper);
  int rand = distr(gen);

  return captions[rand];
}

int main() {
  vector<string> captions = {"caption1", "caption2", "caption3"};
  string caption = caption_randomizer(captions);
  cout << caption << endl;
  caption = caption_randomizer(captions);
  cout << caption << endl;
  caption = caption_randomizer(captions);
  cout << caption << endl;
  caption = caption_randomizer(captions);
  cout << caption << endl;
}
