#include "../include/header.h"

int main() {
  load_env_vars();
  int choice;
  cout << "\n ***************************** FACEBOOK AUTO POST "
          "*******************************\n\n";
  cout << "Enter Ctrl+c to exit...\n\n";
  while (1) {
    cout << " 1.All Pages \n 2.Select Pages\n >>";
    cin >> choice;
    if (choice == 1) {
      family_island();
      coin_tales();
    } else if (choice == 2) {
      int opt;
      cout << "\n 1.Family Island\n 2.Coin Tales\n >>";
      cin >> opt;
      if (opt == 1) {
        family_island();
      } else if (opt == 2) {
        coin_tales();
      } else {
        cout << "Enter valid page!" << endl;
      }
    } else {
      cout << "Enter a valid input!" << endl;
    }
  }
  // string img =
  //     generateImageFromImage("./output/test.jpg", "generate the similar
  //     image");
  // cout << "Image generated successfully at " << img << endl;

  return 0;
}
