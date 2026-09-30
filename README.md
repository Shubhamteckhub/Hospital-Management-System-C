# Hospital Management System (C & MySQL)

A complete console-based **Hospital Management System** project built using **C Language**, **Dev-C++ 5.11**, and **MySQL 8.0 Database**.

---

## Features

### Patient Management
- Add Patient
- View All Patients
- Search Patient by ID
- Update Patient Disease
- Delete Patient

### Doctor Management
- Add Doctor
- View All Doctors
- Search Doctor by ID

### Appointment Management
- Book Appointment
- View Appointments (with SQL JOINs)
- Search Appointment

### Billing Management
- Generate Bill (Consultation Fee + Medicine Charges)
- View All Bills

---

## Tech Stack & Requirements
- **Language**: C
- **IDE**: Dev-C++ 5.11 (TDM-GCC 64-bit)
- **Database**: MySQL 8.0 & MySQL Workbench
- **API**: MySQL C API (`mysql.h`, `libmysql.dll`)

---

## Database Setup
1. Open **MySQL Workbench**.
2. Run the provided `hospital_db23.sql` script to create the database and tables.

---

## How to Compile and Run
1. Open Dev-C++.
2. Open `Project1.dev` or `main.c`.
3. Configure MySQL C API directories in **Project Options**:
   - **Include Directories**: `C:\mysql\include`
   - **Library Directories**: `C:\mysql\lib`
   - **Linker Flag**: `-lmysql`
4. Copy `libmysql.dll` to your project directory.
5. Press **F11** to Compile and Run.

---

## License
This project is open-source and free to use for learning purposes.
