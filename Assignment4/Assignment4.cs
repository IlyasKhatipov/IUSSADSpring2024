using System.Globalization;

// Prototype Pattern
public interface IBook // Defines the prototype interface.
{
    IBook Clone(); 
    string Title { get; }
    string Author { get; } 
    string Price { get; }
    void SetPrice(string price);
}

public class TextualBook : IBook // Concrete implementation of the prototype interface.
{
    public string Title { get; } 
    public string Author { get; } 
    public string Price { get; private set; } 

    public TextualBook(string title, string author, string price) 
    {
        Title = title;
        Author = author;
        Price = price;
    }

    public IBook Clone() => new TextualBook(Title, Author, Price); 

    public void SetPrice(string price) => Price = price; 
}

public class AudioBook : IBook // Another concrete implementation of the prototype interface.
{
    public string Title { get; } 
    public string Author { get; } 
    public string Price { get; private set; } 

    public AudioBook(string title, string author, string price) 
    {
        Title = title;
        Author = author;
        Price = price;
    }

    public IBook Clone() => new AudioBook(Title, Author, Price); 

    public void SetPrice(string price) => Price = price; 
}

public class PrototypeManager // Manages prototypes and implements the clone operation.
{
    private readonly Dictionary<string, IBook> _prototypes = new Dictionary<string, IBook>();

    public void AddPrototype(string key, IBook prototype) => _prototypes[key] = prototype; 

    public IBook GetPrototype(string key) => _prototypes[key].Clone(); 
}

// State Pattern
public enum UserType { Standard, Premium } // Enum representing user types.

public abstract class UserState // Defines the state interface.
{
    public abstract void Subscribe(User user); 
    public abstract void Unsubscribe(User user); 
}

public class SubscribedState : UserState // Concrete state representing subscribed users.
{
    public override void Subscribe(User user) => user.State = new SubscribedState(); 
    public override void Unsubscribe(User user) => user.State = new UnsubscribedState(); 
}

public class UnsubscribedState : UserState // Concrete state representing unsubscribed users.
{
    public override void Subscribe(User user) => user.State = new SubscribedState();
    public override void Unsubscribe(User user) => user.State = new UnsubscribedState();
}

public class User
{
    public string Username { get; }
    public UserType Type { get; }
    public UserState State { get; set; }

    public User(string username, UserType type) 
    {
        Username = username;
        Type = type;
        State = new UnsubscribedState();
    }

    public void Subscribe() => State.Subscribe(this); 
    public void Unsubscribe() => State.Unsubscribe(this);
}

// Facade Pattern
public class BookstoreFacade // Provides a simplified interface to interact with the system.
{
    private readonly Dictionary<string, IBook> _books = new Dictionary<string, IBook>();
    private readonly Dictionary<string, User> _users = new Dictionary<string, User>(); 
    private readonly List<string> _resultStrings = new List<string>();
    private readonly List<string> _subscribedUsers = new List<string>();

    public void CreateTextualBook(string title, string author, string price)
    {
        if (_books.ContainsKey(title))
        {
            _resultStrings.Add("Book already exists");
            return;
        }

        _books[title] = new TextualBook(title, author, price);
    }

    public void CreateAudioBook(string title, string author, string price)
    {
        if (_books.ContainsKey(title))
        {
            _resultStrings.Add("Book already exists");
            return;
        }

        _books[title] = new AudioBook(title, author, price);
    }

    public void CreateUser(string username, UserType type)
    {
        if (_users.ContainsKey(username))
        {
            _resultStrings.Add("User already exists");
            return;
        }

        _users[username] = new User(username, type);
    }

    public void SubscribeUser(string username)
    {
        if (!_users.ContainsKey(username))
        {
            _resultStrings.Add("User does not exist");
            return;
        }

        User user = _users[username];
        if (user.State.GetType() == typeof(SubscribedState))
        {
            _resultStrings.Add("User already subscribed");
            return;
        }

        user.Subscribe();
        _subscribedUsers.Add(username);
    }

    public void UnsubscribeUser(string username)
    {
        if (!_users.ContainsKey(username))
        {
            _resultStrings.Add("User does not exist");
            return;
        }

        User user = _users[username];
        if (user.State.GetType() != typeof(SubscribedState))
        {
            _resultStrings.Add("User is not subscribed");
            return;
        }
        if (user.State.GetType() == typeof(UnsubscribedState))
        {
            _resultStrings.Add("User is already unsubscribed");
            return;
        }

        user.Unsubscribe();
        _subscribedUsers.Remove(username);
    }

    public void UpdateBookPrice(string title, string newPrice)
    {
        if (!_books.ContainsKey(title))
        {
            _resultStrings.Add("Book does not exist");
            return;
        }

        _books[title].SetPrice(newPrice);

        foreach (var username in _subscribedUsers)
        {
            _resultStrings.Add($"{username} notified about price update for {title} to {newPrice}");
        }
    }

    public void ReadBook(string username, string title)
    {
        if (!_users.ContainsKey(username))
        {
            _resultStrings.Add("User does not exist");
            return;
        }

        if (!_books.ContainsKey(title))
        {
            _resultStrings.Add("Book does not exist");
            return;
        }

        _resultStrings.Add($"{username} reading {title} by {_books[title].Author}");
    }

    public void ListenBook(string username, string title)
    {
        if (!_users.ContainsKey(username))
        {
            _resultStrings.Add("User does not exist");
            return;
        }

        if (!_books.ContainsKey(title))
        {
            _resultStrings.Add("Book does not exist");
            return;
        }

        if (_users[username].Type == UserType.Premium)
        {
            _resultStrings.Add($"{username} listening {title} by {_books[title].Author}");
            return;
        }

        _resultStrings.Add("No access");
    }

    public List<string> GetResultStrings() => new List<string>(_resultStrings);
}

class Program
{
    static void Main(string[] args)
    {
        CultureInfo ci = new CultureInfo("en-US");
        CultureInfo.CurrentCulture = ci;
        BookstoreFacade bookstore = new BookstoreFacade();

        string input;
        do
        {
            input = Console.ReadLine();
            if (input != "end")
            {
                string[] parts = input.Split(' ');
                switch (parts[0])
                {
                    case "createBook":
                        bookstore.CreateTextualBook(parts[1], parts[2], parts[3]);
                        break;
                    case "createUser":
                        UserType userType;
                        if (Enum.TryParse(parts[1], true, out userType))
                        {
                            bookstore.CreateUser(parts[2], userType);
                        }
                        else
                        {
                            Console.WriteLine("Invalid user type");
                        }
                        break;
                    case "subscribe":
                        bookstore.SubscribeUser(parts[1]);
                        break;
                    case "unsubscribe":
                        bookstore.UnsubscribeUser(parts[1]);
                        break;
                    case "updatePrice":
                        bookstore.UpdateBookPrice(parts[1], parts[2]);
                        break;
                    case "readBook":
                        bookstore.ReadBook(parts[1], parts[2]);
                        break;
                    case "listenBook":
                        bookstore.ListenBook(parts[1], parts[2]);
                        break;
                    default:
                        Console.WriteLine("Invalid command");
                        break;
                }
            }
        } while (input != "end");

        foreach (var resultString in bookstore.GetResultStrings())
        {
            Console.WriteLine(resultString);
        }
    }
}
