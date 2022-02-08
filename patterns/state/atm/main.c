#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define HAS_FLAGS(flags, query)     ((flags) & (query))

enum session_state_tag {
    S_NULL        = 0,
    S_START       = 1 << 0,
    S_INSERT_CARD = 1 << 1,
    S_ENTER_PIN   = 1 << 2,
    S_MENU        = 1 << 3,
    S_WITHDRAW    = 1 << 4,
    S_DEPOSIT     = 1 << 5,
    S_BALANCE     = 1 << 6,
    S_END         = 1 << 7,
    S_ERROR       = 0xFFFFFFFF,
};

typedef struct {
    enum session_state_tag tag;
    const char *message;
    enum session_state_tag opts;
} State;

static const State state_objs[] = {
    {S_START, "Welcome :)", S_INSERT_CARD},
    {S_INSERT_CARD, "Accepting ATM card...", S_ENTER_PIN},
    {S_ENTER_PIN, "Enter PIN", S_MENU},
    {S_MENU, "Select an option", S_DEPOSIT | S_WITHDRAW | S_BALANCE | S_END},
    {S_WITHDRAW, "Enter an amount to withdraw.", S_MENU},
    {S_DEPOSIT, "Deposit checks and cash", S_MENU},
    {S_BALANCE, "View balance", S_MENU},
    {S_END, "Have a nice day :)", S_NULL},
    {S_ERROR, "Error :(", S_END},
};

struct menu_opt {
    const char *text;
    enum session_state_tag next;
};

static struct menu_opt menu_opts[] = {
    {"Withdraw", S_WITHDRAW},
    {"Deposit", S_DEPOSIT},
    {"View Balance", S_BALANCE},
    {"End", S_END},
};

#define ARR_COUNT(arr)     (sizeof(arr) / sizeof(*(arr)))

typedef struct {
    unsigned long id;
    unsigned short pin_hash;
    int balance;
} Account;

Account account_db[] = {
    {123456789, 2468, 100},
};

typedef struct {
    unsigned long account_id;
    clock_t issued_at;
} LoginToken;

typedef struct {
    const State *curr;
    Account *account;
    int error_status;
} Session;

unsigned short hash_pin(unsigned short raw)
{
    return raw * 2;
}

int token_verify(LoginToken *this)
{
    clock_t now = clock();
    double span = now - this->issued_at;
    double seconds = span / CLOCKS_PER_SEC;
    return seconds < 10;
}

void token_free(LoginToken *this)
{
    free(this);
}

Account *account_lookup(unsigned long account_id)
{
    int len = ARR_COUNT(account_db);
    Account *account = NULL;
    for (int i = 0; i < len; ++i) {
        account = &account_db[i];
        if (account->id == account_id)
            return account;
    }
    return NULL;
}

Account *account_verify(Account *this, unsigned short pin)
{
    return this->pin_hash == hash_pin(pin) ? this : NULL;
}

void session_message(Session *this)
{
    printf("\n########\n\n");
    printf("%s\n", this->curr->message);
}

void session_goto(Session *this, enum session_state_tag tag)
{
    if (!HAS_FLAGS(this->curr->opts, tag)) {
        fprintf(stderr, "Cannot proceed to state %d from %d.\n", tag, this->curr->tag);
        abort();
    }

    for (int i = 0; i < ARR_COUNT(state_objs); ++i) {
        const State *s = &state_objs[i];
        if (s->tag == tag) {
            this->curr = s;
            session_message(this);
            return;
        }
    }

    fprintf(stderr, "Invalid target state: %d\n", tag);
    abort();
}

Session *session_start(void)
{
    Session *new = calloc(sizeof *new, 1);
    new->curr = &state_objs[0];
    new->account = NULL;
    new->error_status = 0;

    session_message(new);
    return new;
}

void session_end(Session *this)
{
    int status = this->error_status;
    session_goto(this, S_END);
    free(this);
    exit(status);
}

LoginToken *session_insert_card(Session *this, unsigned long account_id)
{
    session_goto(this, S_INSERT_CARD);
    LoginToken *token = malloc(sizeof *token);
    token->account_id = account_id;
    token->issued_at = clock();
    return token;
}

void session_error(Session *this, const char *message)
{
    session_goto(this, S_ERROR);
    fprintf(stderr, "Error: %s\n", message);
    this->error_status = -1;
    session_end(this);
}

void session_menu(Session *this)
{
    session_goto(this, S_MENU);

    for (int i = 0; i < ARR_COUNT(menu_opts); ++i) {
        struct menu_opt *item = &menu_opts[i];
        printf("* %s\n", item->text);
    }
}

void session_enter_pin(Session *this, LoginToken *token, unsigned short pin)
{
    session_goto(this, S_ENTER_PIN);

    if (!token_verify(token)) {
        session_error(this, "Cannot verify token");
        return;
    }

    printf("Entering: ****\n");
    Account *account = account_lookup(token->account_id);
    if (!account) {
        session_error(this, "Cannot find account");
        return;
    }
    if (!account_verify(account, pin)) {
        session_error(this, "Cannot verify PIN");
        return;
    }
    this->account = account;

    session_menu(this);
}

void session_deposit(Session *this, int amount)
{
    session_goto(this, S_DEPOSIT);
    printf("Depositing $%d\n", amount);
    this->account->balance += amount;
    session_menu(this);
}

void session_withdraw(Session *this, int amount)
{
    session_goto(this, S_WITHDRAW);
    printf("Withdrawing $%d\n", amount);
    this->account->balance -= amount;
    session_menu(this);
}

void session_balance(Session *this)
{
    session_goto(this, S_BALANCE);
    printf("Balance $%d\n", this->account->balance);
    session_menu(this);
}

int main(void)
{
    Session *session = session_start();

    LoginToken *token = session_insert_card(session, 123456789);
    session_enter_pin(session, token, 1234);
    token_free(token);

    session_balance(session);
    session_deposit(session, 10);
    session_withdraw(session, 3);
    session_balance(session);

    session_end(session);
}
