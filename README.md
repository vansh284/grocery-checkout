# 🛒 Grocery Self-Checkout System

A simulated software interface for a self-service grocery checkout kiosk. This project models the complete customer journey—from language selection and item scanning to bagging and payment—while also providing a robust Admin/Simulation dashboard for testing and troubleshooting.


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

*(Note: Adjust this section to perfectly match your specific language/framework, e.g., Java, Python, React, etc.)*

The codebase is structured to separate the graphical user interface from the underlying business logic and state management.

* `/src`
  * `/components` *(or `/views`)*: Contains the visual elements of the application.
    * `WelcomeScreen` - Handles language selection and initiation.
    * `MainCheckout` - The primary scanning and receipt interface.
    * `ProduceScale` - Interface for weighing fruits/veggies.
    * `PaymentFlow` - Manages the confirmation, bag selection, and payment screens.
  * `/admin` *(or `/sim`)*: Contains the logic and UI for the simulation tab and admin override controls.
  * `/models` *(or `/core`)*: Defines the data structures for `Item`, `Cart`, and `Transaction`.
  * `/controllers` *(or `/state`)*: Manages the central state of the application (e.g., current cart total, active language, admin authorization status).
  * `/assets`: Contains localized strings, icons, and stylesheet data.

---

## 🚀 How to Run

1. Clone the repository: `git clone <LINK_TO_GIT_REPO>`
2. Navigate to the project directory: `cd <PROJECT_FOLDER>`
3. Run the application: `<INSERT_RUN_COMMAND>` *(e.g., `npm start`, `python main.py`, or instructions to compile in your IDE)*.
