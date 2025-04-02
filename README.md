# COMP3400

# UTILITY SERVICE PROVIDER

Assume that a number of independent utility service providers are allowed to market and sell utility products at competitive prices to residential customers. Each provider offers all of the products listed below, but consumers are allowed to choose different types of utility services from different providers.  

## List of Utility Services:
- **Natural Gas**  
- **Internet Service**  
  - TV  
  - Mobile Phone  
  - Home Phone  
- **Hydro**  
  - Electric  
  - Water  
  - Sewerage  

## System Requirements  
Create a system that maintains all the necessary information about:  
- Utilities  
- Customers  
- Income/Sales from utility services  
- Billing (paid/default)  

### Assumptions:  
- Number of providers = **10**  
- Number of customers = **100**  
- Total time for record = **6 months**  
- Payment overdue time = **more than 30 days after bill**  

You can search and find rates for any of these types of utilities. Make sure that billing also includes the fixed components, i.e., meter rate for all types of the above-mentioned utilities.

To run code:  :make clean
              :make
              :./bin/main

Version Control Repo available at:
https://github.com/SteelCode23/COMP3400/


UtilityService
-void createService()
  Creates a new entry via user input.
-UtilityService readServiceById(int)
  Returns a record by its ID.
-void updateService()
  Updates a record’s values via input.
-std::vector<UtilityService> loadServices(...)
  Loads records from file.
-void saveServices(...)
  Saves service data to file.
-void displayServices()
  Displays all services to the console.

BillCalendar
-bool isOpen()
  Returns true if calendar is open.
-bool createBillCalendar()
  Creates a calendar via validated user input.
-vector<BillCalendar> readAllBillCalendars()
  Loads all billing calendars.
-BillCalendar readBillCalendarById(int)
  Gets a calendar by ID.
-void updateBillCalendar()
  Opens/closes a calendar by ID.
-void displayCalendar()
  Displays the currently open calendar.
-void saveCalendars(...)
  Saves calendar data to file.

BatchService
-std::vector<Customer> loadCustomers(...)
  Loads customer data from file.
-std::vector<Usage> loadUsage(...)
  Loads usage for a specific calendar.
-void BillingBatch()
  Generates bills based on usage/rates.
-void SimulatePayments()
  Applies random simulated payments.
-void CalculateOverdue()
  Flags unpaid bills as overdue.
-void saveBills(...)
  Writes multiple bills to file.
-void postPayments(...)
  Appends payments to payments.txt.

BillingSystem
-void addProvider(...)
  Adds a new provider to the system.
-void addCustomer(...)
  Adds a new customer to the system.
-void generateBill(...)
  Unified interface for bill creation.
-void trackOverduePayments()
  Flags overdue bills across system.
-void displayBills()
  Lists all bills in the system.

Date
-std::chrono::year_month_day parseDate(...)
  Parses YYYY-MM-DD into a date object.
-void addDays(int)
  Increments the date by given days.
-bool isOverdue(const Date&)
  Compares with another date to check due.
-void display()
  Displays the date in a readable format.

Bill
-std::vector<Bill> loadBills(...)
  Loads all bill records from file.
-Bill readBillById(int)
  Gets a bill by ID.
-void displayBill()
  Displays all fields of a bill.
-void readAndDisplayBillById(int)
  Looks up and prints bill details.
-void saveBills(...)
  Saves bills with overwrite or append.
-void updateBill()
  Updates a selected bill’s fields.
-void listOverdueBills()
  Displays only overdue bills.
-bool isOverdue(Date)
  Checks if this bill is overdue.
-void generateManagementReport()
  Outputs a summary report of bills.

Customer
-void subscribeService(...)
  Links a service to the customer.
-void addBill(...)
  Adds a bill to the customer.
-void viewBills() const
  Shows all customer billing history.
-void makePayment(int)
  Pays a specified bill.
-static std::vector<Customer> loadCustomers(...)
  Loads customers from file.
-void createCustomer()
  Prompts user to add a customer.
-static void saveCustomers(...)
  Saves customers to file.
-static Customer readCustomerById(int)
  Gets customer by ID.
-void updateCustomer()
  Changes customer info.
-static void displayCustomers()
  Lists all customers.

Rate
-void createRate()
  Adds a new rate for a provider/service.
-Rate readRateById(int)
  Finds a rate using ID.
-void updateRate()
  Modifies rate amounts or attributes.
-std::vector<Rate> loadRates(...)
  Reads all rates from file.
-void saveRates(...)
  Writes all rate entries to file.
-void displayRates()
  Shows all rates to user.

UtilityProvider
-void createProvider()
  Adds a new utility provider.
-UtilityProvider readProviderById(int)
  Finds provider info by ID.
-void updateProvider()
  Changes provider details.
-std::vector<UtilityProvider> loadProviders(...)
  Loads provider data.
-void saveProviders(...)
  Saves to provider file.
-void displayProviders()
  Prints provider list.

Usage
-std::vector<Usage> loadUsage(...)
  Loads usage records from file.
-void simulateUsage()
  Fills system with randomized usage data.
  
PDFGenerator
void PDFGenerator::writeHeader() //Writes header
int PDFGenerator::addObject() //Add a single object
int PDFGenerator::addStreamObject() //Creates a stream object
void PDFGenerator::writeObject() //writes the object
void PDFGenerator::writeStreamObject() //writes the stream object
void PDFGenerator::writeCrossReferenceTable() //Generates the specific format needed to render a pdf
void PDFGenerator::finalize() //Generates final pdf

InvoicePDF
void generateInvoice() //Generates the actual invoice
void generate() //calls generate and finalize
