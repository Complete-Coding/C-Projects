#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <conio.h>
#include <ctype.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

const int BAR_LENGTH  = 55;
const int MAX_TASKS =  1;

#define CREDENTIAL_LENGHT 30
#define LOGIN "Ayushverma"
#define ADMIN_PASS 1234

#define RESET 7
#define PINK 13
#define RED 4
#define GREEN 2
#define BLUE 1
#define YELLOW 14
#define WHITE 15

void setColor(int color){
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

typedef struct{
  char username[CREDENTIAL_LENGHT];
  int account_no;
  float balance;
  int pin;
  int id;
  int progress;
  int step;
} User;

void print_bar(User task){
  int bars_to_show = (task.progress * BAR_LENGTH) / 100 ;
  printf("Fetching User details..\n [");

  for (int i = 0; i<BAR_LENGTH; i++){
    if(i<bars_to_show){
      printf("=");
    } else{
      printf(" ");
    }
  }
  printf("] %d %%\n", task.progress);

}

void clear_screen(){
  #ifdef _WIN32
    system("cls");
  #else
    system("clear");
  #endif
}

void progress_bar(){
                    
    User tasks[MAX_TASKS];

    for(int i = 0; i<MAX_TASKS; i++){
      tasks[i].progress = 0;
      tasks[i].step = 10;
    }
    
    int tasks_incomplete = 1;
    while(tasks_incomplete){
      tasks_incomplete = 0;
      clear_screen();
      for(int i = 0; i<MAX_TASKS; i++){
        tasks[i].progress += tasks[i].step;
        if(tasks[i].progress >100){
          tasks[i].progress =  100;
        } 
        if(tasks[i].progress <100){
          tasks_incomplete = 1;
        }
        print_bar(tasks[i]);
      }
      sleep(1);
    }
}

void print_screen(){
  setColor(BLUE);
  printf("\n\n                 Welcome to Our bank !!!\n");
  setColor(YELLOW);
  printf("\nPlease Select one of the following option: \n");
  printf("1. Create an Account\n");
  printf("2. Deposit Cash\n");
  printf("3. Withdrawl Cash\n");
  printf("4. Ask for help\n");
  printf("5. Show Balance\n");
  printf("6. Delete your Account\n");
  printf("7. Admin Login.\n");
  printf("8. Exit.!\n");
  
  int blink = 1;
  while (1)
  {
    setColor(GREEN);printf("\r%s", blink ? "Please enter your choice: " : "                          ");setColor(RESET);
    blink = !blink;
    fflush(stdout);
    Sleep(700);

    // Check if a key is pressed
    if (_kbhit())
    {
      break;
    }
  }
  printf("\r                           \n");
  setColor(GREEN);printf("Please enter your choice: ");setColor(RESET);

}

void fix_fgets_input(char *string){  // removes \n from the string
  int index = strcspn(string, "\n"); // helps finding the given element
  string[index] = '\0';
}

void create_account(){
  setColor(BLUE);
  printf("\nCREATING ACCOUNT..!\n");
  setColor(RESET);
  User users;
  int attempt = 0;

  setColor(RED);
  FILE *file = fopen("users.txt", "ab+");
  if (file == NULL)
  {
    printf("Error opening file\n");
    return;
  }
  setColor(RESET);

  char c;
  do
  {
    c = getchar();
  } while (c != '\n' && c != EOF);

  setColor(YELLOW);

  printf("\nPlease enter your full name: ");
  setColor(RESET);
  fgets(users.username, CREDENTIAL_LENGHT, stdin);
  fix_fgets_input(users.username);

  while (1){
    setColor(YELLOW);printf("Please create your 5 digit account number: ");setColor(RESET);
    scanf("%d", &users.account_no);
    if (users.account_no < 10000 || users.account_no > 99999){
      setColor(RED);printf("Account number must be of 5 digits!\n");
      if (attempt >= 2){
        printf("\nAttempt falied!.\nExiting System!!, Good Bye..\n");setColor(RESET);
        return;
      }
      attempt++;
      continue;
    } else {
        break;
      }
  }



  while (1){
    setColor(YELLOW);printf("Please create your 4 digit PIN: ");setColor(RESET);
    scanf("%d", &users.pin);
    if (users.pin < 1000 || users.pin > 9999){
      setColor(RED);printf("PIN must be of 4 digits!\n");
      if (attempt >= 2){
        printf("\nAttempt falied!.\nExiting System!!, Good Bye..\n");setColor(RESET);
        return;
      }
      attempt++;
      continue;
    } else {
        break;
      }
  }

  users.balance = 0;

  fwrite(&users, sizeof(users), 1, file);
  fclose(file);
  setColor(BLUE);
  printf("\nAccount created successfully!");
  setColor(RESET);
}

void show_balance(){
  FILE *file = fopen("users.txt", "rb");
  if (file == NULL){
    setColor(RED);("\nError opening file, No account exists yet..\n");setColor(RESET);
    return;
  }

  User acc_read;
  int acc_no;
  setColor(YELLOW);printf("\nPlease enter your account number: ");setColor(RESET);
  scanf("%d", &acc_no);
  if (acc_no < 0)
  {
    setColor(YELLOW);printf("Please enter a valid account Number: \n");setColor(RESET);
    return;
  }

  while (fread(&acc_read, sizeof(acc_read), 1, file)){
    if (acc_read.account_no == acc_no){
      progress_bar();

      setColor(BLUE);printf("Welcome %s!\n", acc_read.username);
      printf("Your Account balance is Rs.%.2f", acc_read.balance);setColor(RESET);
      fclose(file);
      return;
    }
  }
  fclose(file);
  setColor(RED);printf("\nAccount no: %d was not found.\n", acc_no);setColor(RESET);
}

void deposit_money(){
  FILE *file = fopen("users.txt", "r+b");
  if (file == NULL)
  {
    return;
  }

  User deposit;
  int acc_no;
  float money;
  setColor(YELLOW);printf("\nPlease enter your account number: ");setColor(RESET);
  scanf("%d", &acc_no);
  if (acc_no <= 0)
  {
    setColor(RED);printf("Please enter a valid account Number: \n");setColor(RESET);
    return;
  }

  while (fread(&deposit, sizeof(deposit), 1, file))
  {
    if (acc_no == deposit.account_no)
    {
      setColor(YELLOW);printf("Enter the amount you want to deposit: ");setColor(RESET);
      scanf("%f", &money);

      if (money <= 0)
      { // Prevent negative deposits
        setColor(RED);printf("Invalid deposit amount!\n");setColor(RESET);
        fclose(file);
        return;
      }
      deposit.balance += money;

      // Move file pointer back to the start of this record
      fseek(file, -(long)sizeof(deposit), SEEK_CUR);

      // Write the updated record back
      fwrite(&deposit, sizeof(deposit), 1, file);

      setColor(BLUE);printf("\nCongrats!! %s, \nRs. %.2f is deposited to your account.\n", deposit.username, money);setColor(RESET);
      fclose(file);
      return;
    }
  }

  fclose(file);
  setColor(RED);printf("\nAccount %d doesn't exist.\n", acc_no);setColor(RESET);
}

void Withdrawl_money(){
  FILE *file = fopen("users.txt", "r+b");
  if (file == NULL){
    setColor(RED);("Fatal Error!\n");setColor(RESET);
    return;
  }

  User withdraw;
  int acc_no;
  float money;
  int pin;

  setColor(YELLOW);printf("\nPlease enter your account number: ");setColor(RESET);
  scanf("%d",&acc_no);
  setColor(YELLOW);printf("Please enter your 4 digit PIN: ");setColor(RESET);
  scanf("%d",&pin);

  while (fread(&withdraw, sizeof(withdraw), 1, file)){

    if (acc_no == withdraw.account_no && pin == withdraw.pin){

      setColor(YELLOW);printf("Enter the amount you want to withdraw: ");setColor(RESET);
      scanf("%f", &money);

      if (money <= 0)
      { // Prevent negative deposits
        setColor(RED);printf("Invalid deposit amount!\n");
        fclose(file);
        return;
      }

      if (withdraw.balance < money)
      {
        printf("\nInsufficient Balance!");
        fclose(file);
        return;
      }setColor(RESET);
      withdraw.balance -= money;

      // Move file pointer back to the start of this record
      fseek(file, -(long)sizeof(withdraw), SEEK_CUR);

      // Write the updated record back
      fwrite(&withdraw, sizeof(withdraw), 1, file);
      setColor(PINK);
      printf("\nHey! %s,\nRs. %.2f has been withdrawl from your account.\n", withdraw.username, money); setColor(RESET);
      fclose(file);
      return;
    }
  }

  fclose(file);
  setColor(RED);printf("Invalid PIN or account number.\n");setColor(RESET);
}

void permanently_delete(){
  FILE *file = fopen("users.txt", "rb");
  if (file == NULL)
  {
    printf("Error opening file.\n");
    return;
  }

  FILE *temp = fopen("temp.txt", "wb");
  if (temp == NULL)
  {
    printf("Error opening file.\n");
    return;
  }
  User user;
  int delete_count = 0;

  while (fread(&user, sizeof(user), 1, file))
  {
    if (user.account_no == -1)
    {
      delete_count++;
    }
    else
    {
      fwrite(&user, sizeof(user), 1, temp);
    }
  }
  fclose(file);
  fclose(temp);

  if (delete_count > 0)
  {
    remove("users.txt");
    rename("temp.txt", "users.txt");
    setColor(BLUE);printf("%d closed accounts deletd permanently!\n", delete_count);
  }
  else
  {
    remove("temp.txt");
    printf("No closed accounts found to delete.\n");setColor(RESET);
  }
}

void delete_account(){
  FILE *file = fopen("users.txt", "r+b");
  if (file == NULL)
  {
    printf("Error opening file, file doesn't exist.\n");
    return;
  }

  User delete;
  int acc_no;
  int pin;
  printf("\nPlease enter your account number: ");
  scanf("%d", &acc_no);
  printf("Please enter your 4 digit PIN: ");
  scanf("%d",&pin);

  while (fread(&delete, sizeof(delete), 1, file))
  {
    if (acc_no == delete.account_no && pin == delete.pin)
    {
      if (delete.balance > 0)
      {
        printf("Sorry %s , We are unable to close your account.\n", delete.username);
        printf("Please withdraw all your money before closing your Account.\n");
        return;
      }
      delete.account_no = -1; // doesn't completely delete we will just ingnore the account no. -1

      fseek(file, -(long)sizeof(delete), SEEK_CUR); // Move back to update
      fwrite(&delete, sizeof(delete), 1, file);
      fclose(file);

      setColor(PINK);printf("\nThanks for your time (Mr./Mrs.) %s.. \nYour Account has been deleted successfully \nHave a nice day!\n", delete.username); setColor(RESET);
      return;
    }
  }
  fclose(file);
  setColor(RED);printf("Invalid PIN or account number.\n");setColor(RESET);
}

void Admin_login(){
  User admin;

  FILE *file = fopen("users.txt", "r");
  if (file == NULL)
  {
    setColor(RED);printf("Error opening file.\n");
    return;
  }

  char login[CREDENTIAL_LENGHT];
  int pass;
  int choice;
  int count = 1;
  setColor(BLUE);
  printf("\n                     --Logging as ADMIN--");
  getchar();
  setColor(YELLOW);
  printf("\nPlease enter your Username");
  setColor(RED);
  printf("(case-sensitive): ");
  setColor(RESET);
  fgets(login, sizeof(login), stdin);
  fix_fgets_input(login);
  setColor(YELLOW);
  printf("Please enter your password: ");
  setColor(RESET);
  scanf("%d", &pass);

  if (strcmp(login, LOGIN) == 0 && pass == ADMIN_PASS)
  {
    progress_bar();
    setColor(BLUE);
    printf("\nAdmin Access Granted. Displaying All Users:\n");
    setColor(PINK);
    printf("x------------------------------x----------------------------x\n");
    setColor(RESET);

    while (fread(&admin, sizeof(User), 1, file))
    {
      printf("%d. Name: %s, ", count, admin.username);
      printf("Account Number: %d, ", admin.account_no);
      printf("Balance: Rs. %.2f;\n", admin.balance);
      count++;
    }
    setColor(PINK);
    printf("x------------------------------x----------------------------x");
    setColor(RESET);
    fclose(file);

    setColor(GREEN);printf("\nDo you want to permanently delete all closed accounts ?\n");
    printf("Enter 1 to confirm, any other key to cancel: ");setColor(RESET);
    scanf("%d", &choice);
    getchar();

    if (choice == 1)
    {
      permanently_delete();
    }
    else
    {
      setColor(RED);printf("Exiting..!\n");
      return;
    }
  }
  else
  {
    setColor(RED);printf("\nInvalid Id or passowrd\n");setColor(RESET);
    fclose(file);
    return;
  }
  fclose(file);
  setColor(RESET);
}

int main(){
  int choice;
  while (1)
  {
    print_screen();
    scanf("%d", &choice);setColor(RESET);
    switch (choice)
    {
    case 1:
      create_account();
      break;
    case 2:
      deposit_money();
      break;
    case 3:
      Withdrawl_money();
      break;
    case 4:
      printf("This area is under development..\n");
      break;
    case 5:
      show_balance();
      break;
    case 6:
      delete_account();
      break;
    case 7:
      Admin_login();
      break;
    case 8:
      setColor(BLUE);printf("\nExiting System!!\n");setColor(RESET);
      return 0;
    default:
      setColor(RED);printf("Please enter a valid operator.\n");setColor(RESET);
      break;
    }
  }
}
