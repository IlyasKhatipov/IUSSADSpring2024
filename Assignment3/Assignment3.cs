using Console = System.Console;
using System.Globalization;

namespace SSAD_assidnment_3
{
    // Decorator interface
    public interface IAccountDecorator
    {
        void Execute();
    }

    // Concrete decorator class for logging transactions
    public class TransactionLoggerDecorator : IAccountDecorator
    {
        private Account _account;
        private string _transaction;

        public TransactionLoggerDecorator(Account account, string transaction)
        {
            _account = account;
            _transaction = transaction;
        }

        public void Execute()
        {
            if (_transaction.Split(" ")[0] != "Error:")
            {
                _account.History.Add(_transaction);   
            }
        }
    }
    // Strategy pattern interface
    public interface IOperationStrategy
    {
        string ExecuteOperation(float amount, List<Account> participants);
    }

    // Concrete strategies. Each strategy uses decorator for logging it ti accounts history
    public class DepositStrategy : IOperationStrategy
    {
        public string ExecuteOperation(float amount, List<Account> participants)
        {
            Account account = participants[0];
            account.Balance += amount;
            IAccountDecorator decorator = new TransactionLoggerDecorator(account, $"Deposit ${amount:.000}");
            decorator.Execute();
            return $"{account.Name} successfully deposited ${amount:.000}. New Balance: ${account.Balance:.000}.";
        }
    }

    public class WithdrawalStrategy : IOperationStrategy
    {
        public string ExecuteOperation(float amount, List<Account> participants)
        {
            Account account = participants[0];
            if (account.State == "Inactive")
            {
                return $"Error: Account {account.Name} is inactive.";
            }

            float commission = amount * (account.Fee / 100);
            float withdrawnAmount = amount - commission;
            if (amount > account.Balance)
            {
                return $"Error: Insufficient funds for {account.Name}.";
            }
            else
            {
                account.Balance -= amount;
                IAccountDecorator decorator = new TransactionLoggerDecorator(account, $"Withdrawal ${amount:.000}");
                decorator.Execute();
                return $"{account.Name} successfully withdrew ${withdrawnAmount:F3}. New Balance: ${account.Balance:F3}. Transaction Fee: ${commission:F3} ({(account.Fee != 0 ? account.Fee.ToString("F1") : "0")}%) in the system.";

            }
        }
    }

    public class TransferStrategy : IOperationStrategy
    {
        public string ExecuteOperation(float amount, List<Account> participants)
        {
            Account fromAccount = participants[0];
            if (fromAccount.State == "Inactive")
            {
                return $"Error: Account {fromAccount.Name} is inactive.";
            }
            Account toAccount = participants[1];
            float commission = amount * (fromAccount.Fee / 100);
            float transferredAmount = amount - commission;
            if (amount > fromAccount.Balance)
            {
                return $"Error: Insufficient funds for {fromAccount.Name}.";
            }
            else
            {
                fromAccount.Balance -= amount;
                toAccount.Balance += transferredAmount;
                IAccountDecorator decorator = new TransactionLoggerDecorator(fromAccount, $"Transfer ${amount:.000}");
                decorator.Execute();
                return $"{fromAccount.Name} successfully transferred ${transferredAmount:F3} to {toAccount.Name}. New Balance: ${fromAccount.Balance:F3}. Transaction Fee: ${commission:F3} ({fromAccount.Fee:F1}%) in the system.";
            }
        }
    }

    // Account class
    public class Account
    {
        public string Name;
        public string State;
        public float Balance;
        public string Type;
        public float Fee;
        public List<string> History;
        // Constructor with defining transaction fee depending on accoutn's type
        public Account(string name, float balance, string type)
        {
            this.Name = name;
            this.Balance = balance;
            History = new List<string>();
            this.State = "Active";
            this.Type = type;
            History.Add($"Initial Deposit ${balance:.000}");
            if (type == "Savings")
            {
                this.Fee = 1.5f;
            }
            else if (type == "Checking")
            {
                this.Fee = 2.0f;
            }
            else if (this.Type == "Business")
            {
                this.Fee = 2.5f;
            }
        }
        // function for view operation
        public void View()
        {
            string historyString = string.Join(", ", History);
            Console.WriteLine($"{Name}'s Account: Type: {Type}, Balance: ${Balance:F3}, State: {State}, Transactions: [{historyString}].");
        }

    }
    // Singleton class bank through which most of the operations will be performed
    public sealed class Bank
    {
        private static Bank _instance;
        private Bank() { }
        public static Bank GetInstance()
        {
            if (_instance == null)
            {
                _instance = new Bank();
            }
            return _instance;
        }
        private Dictionary<string, Account> _users = new Dictionary<string, Account>();
        public void AddUser(string userName, Account user)
        {
            if (!_users.ContainsKey(userName))
            {
                _users.Add(userName, user);
                Console.WriteLine($"A new {user.Type} account created for {userName} with an initial balance of ${user.Balance:.000}.");
            }
            else
            {
                Console.WriteLine($"Error: Account {userName} already exists");
            }
        }

        public Account GetAccount(string userName)
        {
            if (_users.ContainsKey(userName))
            {
                return _users[userName];
            }
            return null;
        }
        // function that shows history of user's account if it exists
        public void Show(string userName)
        {
            if (_users.ContainsKey(userName))
            {
                _users[userName].View();
            }
            else
            {
                Console.WriteLine($"Error: Account {userName} does not exist.");
            }
        }
        // function that deactivates account
        public void Deactivate(string userName)
        {
            if (_users.ContainsKey(userName))
            {
                if (!_users[userName].State.Equals("Inactive"))
                {
                    _users[userName].State = "Inactive";
                    Console.WriteLine($"{userName}'s account is now deactivated.");
                }
                else
                {
                    Console.WriteLine($"Error: Account {userName} is already deactivated.");
                }
            }
            else
            {
                Console.WriteLine($"Error: Account {userName} does not exist.");
            }
        }
        // function that activates account
        public void Activate(string userName)
        {
            if (_users.ContainsKey(userName))
            {
                if (!_users[userName].State.Equals("Active"))
                {
                    _users[userName].State = "Active";
                    Console.WriteLine($"{userName}'s account is now activated.");
                }
                else
                {
                    Console.WriteLine($"Error: Account {userName} is already activated.");
                }
            }
            else
            {
                Console.WriteLine($"Error: Account {userName} does not exist.");
            }
        }
        // function that will perform operations using concrete strategies
        public void ExecuteOperation(string userName, IOperationStrategy operationStrategy, float amount, List<Account> participants)
        {
            if (_users.ContainsKey(userName))
            {
                string operation = operationStrategy.ExecuteOperation(amount, participants);
                Console.WriteLine(operation);
            }
            else
            {
                Console.WriteLine($"Error: Account {userName} does not exist.");
            }
        }
    }

    public class Program
    {
        public static void Main(string[] args)
        {
            // Setting more suiting precision, getting instance of Bank and reading number of commands
            CultureInfo.CurrentCulture = CultureInfo.InvariantCulture;
            Bank bank = Bank.GetInstance();
            int n = int.Parse(Console.ReadLine());
            // Loop with reading commands and handling every type of command
            for (int i = 0; i < n; i++)
            {
                string[] cfg = Console.ReadLine().Split(" ");
                string cmd = cfg[0];
                switch (cmd)
                {
                    case "Create":
                        string type = cfg[2];
                        string name = cfg[3];
                        float initial = float.Parse(cfg[4]);
                        Account rookie = new Account(name, initial, type);
                        bank.AddUser(name, rookie);
                        break;
                    case "Deposit":
                        DepositStrategy depStrat = new DepositStrategy();
                        string dname = cfg[1];
                        float damount = float.Parse(cfg[2]);
                        List<Account> dparts = new List<Account>();
                        dparts.Add(bank.GetAccount(dname));
                        bank.ExecuteOperation(dname, depStrat, damount, dparts);
                        break;
                    case "Withdraw":
                        string wname = cfg[1];
                        float wamount = float.Parse(cfg[2]);
                        WithdrawalStrategy withStrat = new WithdrawalStrategy();
                        List<Account> wparts = new List<Account>();
                        wparts.Add(bank.GetAccount(wname));
                        bank.ExecuteOperation(wname, withStrat, wamount, wparts);
                        break;
                    case "Transfer":
                        string tsname = cfg[1];
                        string trname = cfg[2];
                        float tamount = float.Parse(cfg[3]);
                        TransferStrategy transfStrat = new TransferStrategy();
                        List<Account> tparts = new List<Account>();
                        if (bank.GetAccount(tsname) != null)
                        {
                            if (bank.GetAccount(trname) != null)
                            {
                                tparts.Add(bank.GetAccount(tsname));
                                tparts.Add(bank.GetAccount(trname));
                                bank.ExecuteOperation(tsname, transfStrat, tamount, tparts);
                            }
                            else
                            {
                                Console.WriteLine($"Error: Account {trname} does not exist.");
                            }
                        }
                        else
                        {
                            Console.WriteLine($"Error: Account {tsname} does not exist.");
                        }

                        break;
                    case "View":
                        string vname = cfg[1];
                        bank.Show(vname);
                        break;
                    case "Deactivate":
                        string dename = cfg[1];
                        bank.Deactivate(dename);
                        break;
                    case "Activate":
                        string acname = cfg[1];
                        bank.Activate(acname);
                        break;

                }

            }
        }
    }
}
