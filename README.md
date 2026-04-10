# 🛒 Grocery Self-Checkout System
By Vansh Nath, Yue Jia, Yongyan Luo, Lylia Mesa

A simulated software interface for a self-service grocery checkout kiosk. This project models the complete customer journey—from language selection and item scanning to bagging and payment—while also providing a robust Admin/Simulation dashboard for testing and troubleshooting.

**Git Repository:** ([Link to our repository](https://github.com/vansh284/grocery-checkout.git))


## 🌟 Implemented Functionality

The system is divided into two primary modes: the **Customer Flow** and the **Admin/Simulation Mode**. 

### 🧍 Customer Flow
* **Initialization:** Users begin by selecting their preferred language and clicking "Start Scanning" to launch the main interface.
* **Item Entry:** * **Standard Scanning:** Items are added to the cart (simulated via the Sim tab).
  * **Produce/Weighted Items:** Dedicated menus allow users to weigh and add bulk items like fruits, vegetables, and bread.
* **Persistent Support:** A "Help" button is anchored at the top of *every* screen, allowing the customer to call an attendant at any point.
* **Secure Checkout Process:**
  * **Confirmation Guard:** Clicking "Pay" triggers a modal asking, *"Have you scanned everything?"* to prevent accidental misclicks.
  * **Bag Selection:** Users can select the number of bags they need before paying.
  * **Payment Methods:** Full support for both Cash and Card payment flows.

### 🛠️ Admin & Simulation Mode
Accessible via the hidden "Sim" tab, this mode allows instructors or developers to manipulate the system state:
* **Item Management:** Simulates item scanning to populate the customer's cart.
* **Override Controls (Admin):** * Delete specific items from the cart (via the 'X' icon).
  * Manually add items by inputting a barcode or custom price.
* **System Controls:** Admin logout functionality and a master switch to turn the machine "Off".

---

## 🖥️ Interface Description

The user interface is designed to mimic modern kiosk touchscreens, prioritizing large touch targets, clear navigation, and error prevention.

1. **Welcome Screen:** Features a clean layout with language toggles and a prominent "Start Scanning" call-to-action.
2. **Main Checkout Dashboard:** * **Left/Center Panel:** Displays the running receipt of scanned items with individual prices and a running total.
   * **Right/Action Panel:** Contains buttons for "Add Produce/Weighted Items," "Pay," and system-level actions.
   * **Top Header:** Houses the persistent "Help/Call Admin" button.
3. **Produce Menu:** A grid-based selection screen categorized by item type (Fruit, Veggie, Bakery) that mimics a digital scale interface.
4. **Payment Flow Modals:** Sequential overlays guiding the user through scan confirmation, bag addition, and tender type selection.
5. **Sim/Admin Dashboard (Dev Mode):** A distinct, utilitarian control panel overlaid or tabbed separately from the main UI, providing form inputs for manual barcodes/prices and system state controls.

---

## 📁 Code Organization

This application is built using **C++ and the Qt Framework**. The architecture heavily utilizes a page-based navigation system to transition smoothly between the different checkout states.

```text
└── grocery-checkout
    ├── LICENSE
    ├── README.md
    ├── grocery-checkout.pro         # Qt Project configuration file
    ├── grocery-checkout_fr_FR.ts    # Qt Translation file (French localization support)
    ├── res.qrc                      # Qt Resource collection file for managing assets
    │
    ├── Core & Navigation
    │   ├── main.cpp                 # Application entry point
    │   ├── mainwindow.cpp / .h / .ui# Main application window and layout container
    │   └── page.cpp / .h            # Base UI class inherited by all specific application views
    │
    ├── State & Configuration
    │   ├── cartdata.h               # Data model defining the shopping cart and item structures
    │   ├── config.h                 # Global application configurations and constants
    │   └── style.h                  # Centralized UI styling and visual definitions
    │
    ├── User Interface Views (Pages)
    │   ├── welcomepage.cpp / .h     # Initial language selection and "Start" screen
    │   ├── mainpage.cpp / .h        # Primary scanning interface and simulation dashboard
    │   ├── categorypage.cpp / .h    # High-level produce categories (Fruit, Veggie, Bread)
    │   ├── itemdetailpage.cpp / .h  # Specific item selection and weighing interface
    │   ├── paymentpage.cpp / .h     # Payment flow, bag selection, and tender types
    │   ├── receiptpage.cpp / .h     # Post-transaction summary screen
    │   └── helppage.cpp / .h        # Persistent support modal to call the attendant
    │
    ├── Admin & Tools
    │   ├── adminpage.cpp / .h       # Admin dashboard (override controls, machine power)
    │   └── numpaddialog.cpp / .h    # Custom on-screen numpad for manual barcode/price entry
    │
    └── img/                         # Embedded visual assets
        ├── earth.png                # Language/Localization icon
        ├── fr.png                   # French language toggle icon
        └── uk.png                   # English language toggle icon
```

### Component Breakdown
* **Data Management:** `cartdata.h` acts as the single source of truth for the customer's current transaction, keeping the UI decoupled from the underlying business logic.
* **Navigation:** `mainwindow.cpp` manages the transitions between various sub-pages (which all inherit from the base `page.h` class), ensuring a seamless kiosk experience.
* **Localization:** Language selection is handled natively via Qt's internationalization tools (`.ts` files), allowing dynamic UI updates based on user preference on the `welcomepage`.
* **Admin Flow:** Protected admin features are encapsulated in `adminpage.cpp`, supplemented by `numpaddialog.cpp` to ensure touchscreen-friendly data entry without requiring a physical keyboard.

---

## 🚀 How to Run
* Option 1 
1. Clone the repository: `git clone https://github.com/vansh284/grocery-checkout.git`
2. Open `grocery-checkout.pro` in **Qt Creator**.
3. Configure the project for your local build kit (e.g., Desktop Qt/MinGW, MSVC, or GCC).
4. Build (Using qMake) and Run the application.
