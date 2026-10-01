#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

/* MySQL Connection Handle (Global pointer) */
MYSQL *conn;

/* Database Credentials - Change these as needed */
#define MYSQL_HOST "localhost"
#define MYSQL_USER "root"
#define MYSQL_PASSWORD "shub1432"  
#define MYSQL_DATABASE "hospital_db23"
#define MYSQL_PORT 3300                 

/* Function Prototypes */
int connectDatabase(void);
void addPatient(void);
void viewPatients(void);
void searchPatient(void);
void updatePatient(void);
void deletePatient(void);
void addDoctor(void);
void viewDoctors(void);
void searchDoctor(void);
void bookAppointment(void);
void viewAppointments(void);
void searchAppointment(void);
void generateBill(void);
void viewBills(void);

/* ========================================================================= */
/* 1. DATABASE CONNECTION FUNCTION                                           */
/* ========================================================================= */
int connectDatabase(void)
{
    conn = mysql_init(NULL);

    if (conn == NULL)
    {
        printf("\n[ERROR] MySQL initialization failed!\n");
        return 0;
    }

    /* Connect to MySQL Database */
    if (mysql_real_connect(conn, MYSQL_HOST, MYSQL_USER, MYSQL_PASSWORD, MYSQL_DATABASE, MYSQL_PORT, NULL, 0) == NULL)
    {
        printf("\n[ERROR] Database connection failed!\n");
        printf("MySQL Error: %s\n", mysql_error(conn));
        return 0;
    }

    return 1;
}

/* ========================================================================= */
/* 2. PATIENT MANAGEMENT FUNCTIONS                                           */
/* ========================================================================= */

/* Add Patient */
void addPatient(void)
{
    int id, age;
    char name[100], gender[20], disease[100];
    char query[500];

    printf("\n========================================\n");
    printf("             ADD PATIENT                \n");
    printf("========================================\n");

    printf("Enter Patient ID: ");
    scanf("%d", &id);

    printf("Enter Patient Name: ");
    scanf(" %[^\n]", name);

    printf("Enter Age: ");
    scanf("%d", &age);

    printf("Enter Gender (Male/Female/Other): ");
    scanf("%s", gender);

    printf("Enter Disease: ");
    scanf(" %[^\n]", disease);

    sprintf(query,
            "INSERT INTO patients (id, name, age, gender, disease) "
            "VALUES (%d, '%s', %d, '%s', '%s')",
            id, name, age, gender, disease);

    if (mysql_query(conn, query) == 0)
    {
        printf("\n[SUCCESS] Patient record added successfully!\n");
    }
    else
    {
        printf("\n[ERROR] Failed to add patient: %s\n", mysql_error(conn));
    }
}

/* View All Patients */
void viewPatients(void)
{
    MYSQL_RES *result;
    MYSQL_ROW row;

    printf("\n=======================================================================\n");
    printf("                           ALL PATIENTS LIST                           \n");
    printf("=======================================================================\n");

    if (mysql_query(conn, "SELECT id, name, age, gender, disease FROM patients") != 0)
    {
        printf("[ERROR] Failed to fetch patients: %s\n", mysql_error(conn));
        return;
    }

    result = mysql_store_result(conn);

    if (result == NULL)
    {
        printf("[INFO] No patient records found in database.\n");
        return;
    }

    printf("%-8s %-20s %-8s %-10s %-20s\n", "ID", "Name", "Age", "Gender", "Disease");
    printf("-----------------------------------------------------------------------\n");

    int count = 0;
    while ((row = mysql_fetch_row(result)))
    {
        printf("%-8s %-20s %-8s %-10s %-20s\n", row[0], row[1], row[2], row[3], row[4]);
        count++;
    }

    if (count == 0)
    {
        printf("[INFO] No patients registered yet.\n");
    }

    mysql_free_result(result);
}

/* Search Patient by ID */
void searchPatient(void)
{
    int id;
    char query[250];
    MYSQL_RES *result;
    MYSQL_ROW row;

    printf("\n========================================\n");
    printf("             SEARCH PATIENT             \n");
    printf("========================================\n");

    printf("Enter Patient ID: ");
    scanf("%d", &id);

    sprintf(query, "SELECT id, name, age, gender, disease FROM patients WHERE id = %d", id);

    if (mysql_query(conn, query) != 0)
    {
        printf("[ERROR] Search query failed: %s\n", mysql_error(conn));
        return;
    }

    result = mysql_store_result(conn);

    if (result == NULL)
    {
        printf("[ERROR] Failed to retrieve data.\n");
        return;
    }

    row = mysql_fetch_row(result);

    if (row != NULL)
    {
        printf("\n--- Patient Found ---\n");
        printf("ID      : %s\n", row[0]);
        printf("Name    : %s\n", row[1]);
        printf("Age     : %s\n", row[2]);
        printf("Gender  : %s\n", row[3]);
        printf("Disease : %s\n", row[4]);
    }
    else
    {
        printf("\n[INFO] Patient with ID %d not found.\n", id);
    }

    mysql_free_result(result);
}

/* Update Patient Disease */
void updatePatient(void)
{
    int id;
    char disease[100];
    char query[300];

    printf("\n========================================\n");
    printf("             UPDATE PATIENT             \n");
    printf("========================================\n");

    printf("Enter Patient ID to update: ");
    scanf("%d", &id);

    printf("Enter New Disease/Diagnosis: ");
    scanf(" %[^\n]", disease);

    sprintf(query, "UPDATE patients SET disease = '%s' WHERE id = %d", disease, id);

    if (mysql_query(conn, query) == 0)
    {
        if (mysql_affected_rows(conn) > 0)
        {
            printf("\n[SUCCESS] Patient record updated successfully!\n");
        }
        else
        {
            printf("\n[INFO] Patient ID %d not found.\n", id);
        }
    }
    else
    {
        printf("\n[ERROR] Update failed: %s\n", mysql_error(conn));
    }
}

/* Delete Patient */
void deletePatient(void)
{
    int id;
    char query[200];

    printf("\n========================================\n");
    printf("             DELETE PATIENT             \n");
    printf("========================================\n");

    printf("Enter Patient ID to delete: ");
    scanf("%d", &id);

    sprintf(query, "DELETE FROM patients WHERE id = %d", id);

    if (mysql_query(conn, query) == 0)
    {
        if (mysql_affected_rows(conn) > 0)
        {
            printf("\n[SUCCESS] Patient record deleted successfully!\n");
        }
        else
        {
            printf("\n[INFO] Patient ID %d not found.\n", id);
        }
    }
    else
    {
        printf("\n[ERROR] Delete failed: %s\n", mysql_error(conn));
    }
}

/* ========================================================================= */
/* 3. DOCTOR MANAGEMENT FUNCTIONS                                            */
/* ========================================================================= */

/* Add Doctor */
void addDoctor(void)
{
    int id;
    char name[100], specialization[100];
    char query[400];

    printf("\n========================================\n");
    printf("              ADD DOCTOR                \n");
    printf("========================================\n");

    printf("Enter Doctor ID: ");
    scanf("%d", &id);

    printf("Enter Doctor Name: ");
    scanf(" %[^\n]", name);

    printf("Enter Specialization (e.g. Cardiology): ");
    scanf(" %[^\n]", specialization);

    sprintf(query,
            "INSERT INTO doctors (id, name, specialization) "
            "VALUES (%d, '%s', '%s')",
            id, name, specialization);

    if (mysql_query(conn, query) == 0)
    {
        printf("\n[SUCCESS] Doctor added successfully!\n");
    }
    else
    {
        printf("\n[ERROR] Failed to add doctor: %s\n", mysql_error(conn));
    }
}

/* View All Doctors */
void viewDoctors(void)
{
    MYSQL_RES *result;
    MYSQL_ROW row;

    printf("\n=========================================================\n");
    printf("                   ALL DOCTORS LIST                      \n");
    printf("=========================================================\n");

    if (mysql_query(conn, "SELECT id, name, specialization FROM doctors") != 0)
    {
        printf("[ERROR] Failed to fetch doctors: %s\n", mysql_error(conn));
        return;
    }

    result = mysql_store_result(conn);

    if (result == NULL)
    {
        printf("[INFO] No doctor records found.\n");
        return;
    }

    printf("%-8s %-25s %-25s\n", "ID", "Doctor Name", "Specialization");
    printf("---------------------------------------------------------\n");

    int count = 0;
    while ((row = mysql_fetch_row(result)))
    {
        printf("%-8s %-25s %-25s\n", row[0], row[1], row[2]);
        count++;
    }

    if (count == 0)
    {
        printf("[INFO] No doctors found in database.\n");
    }

    mysql_free_result(result);
}

/* Search Doctor by ID */
void searchDoctor(void)
{
    int id;
    char query[250];
    MYSQL_RES *result;
    MYSQL_ROW row;

    printf("\n========================================\n");
    printf("             SEARCH DOCTOR              \n");
    printf("========================================\n");

    printf("Enter Doctor ID: ");
    scanf("%d", &id);

    sprintf(query, "SELECT id, name, specialization FROM doctors WHERE id = %d", id);

    if (mysql_query(conn, query) != 0)
    {
        printf("[ERROR] Search query failed: %s\n", mysql_error(conn));
        return;
    }

    result = mysql_store_result(conn);

    if (result == NULL)
    {
        printf("[ERROR] Failed to retrieve data.\n");
        return;
    }

    row = mysql_fetch_row(result);

    if (row != NULL)
    {
        printf("\n--- Doctor Details ---\n");
        printf("ID             : %s\n", row[0]);
        printf("Name           : %s\n", row[1]);
        printf("Specialization : %s\n", row[2]);
    }
    else
    {
        printf("\n[INFO] Doctor with ID %d not found.\n", id);
    }

    mysql_free_result(result);
}

/* ========================================================================= */
/* 4. APPOINTMENT MANAGEMENT FUNCTIONS                                       */
/* ========================================================================= */

/* Book Appointment */
void bookAppointment(void)
{
    int patient_id, doctor_id;
    char date[20];
    char query[400];

    printf("\n========================================\n");
    printf("            BOOK APPOINTMENT            \n");
    printf("========================================\n");

    printf("Enter Patient ID: ");
    scanf("%d", &patient_id);

    printf("Enter Doctor ID: ");
    scanf("%d", &doctor_id);

    printf("Enter Appointment Date (YYYY-MM-DD): ");
    scanf("%s", date);

    sprintf(query,
            "INSERT INTO appointments (patient_id, doctor_id, appointment_date) "
            "VALUES (%d, %d, '%s')",
            patient_id, doctor_id, date);

    if (mysql_query(conn, query) == 0)
    {
        printf("\n[SUCCESS] Appointment booked successfully!\n");
    }
    else
    {
        printf("\n[ERROR] Failed to book appointment: %s\n", mysql_error(conn));
        printf("Note: Ensure Patient ID and Doctor ID exist in the database!\n");
    }
}

/* View Appointments */
void viewAppointments(void)
{
    MYSQL_RES *result;
    MYSQL_ROW row;

    char query[] =
        "SELECT appointments.id, patients.name, doctors.name, appointments.appointment_date "
        "FROM appointments "
        "JOIN patients ON appointments.patient_id = patients.id "
        "JOIN doctors ON appointments.doctor_id = doctors.id";

    printf("\n=======================================================================\n");
    printf("                          ALL APPOINTMENTS                             \n");
    printf("=======================================================================\n");

    if (mysql_query(conn, query) != 0)
    {
        printf("[ERROR] Failed to fetch appointments: %s\n", mysql_error(conn));
        return;
    }

    result = mysql_store_result(conn);

    if (result == NULL)
    {
        printf("[INFO] No appointments found.\n");
        return;
    }

    printf("%-8s %-20s %-20s %-15s\n", "Appt ID", "Patient Name", "Doctor Name", "Date");
    printf("-----------------------------------------------------------------------\n");

    int count = 0;
    while ((row = mysql_fetch_row(result)))
    {
        printf("%-8s %-20s %-20s %-15s\n", row[0], row[1], row[2], row[3]);
        count++;
    }

    if (count == 0)
    {
        printf("[INFO] No booked appointments in database.\n");
    }

    mysql_free_result(result);
}

/* Search Appointment by Patient ID */
void searchAppointment(void)
{
    int patient_id;
    char query[400];
    MYSQL_RES *result;
    MYSQL_ROW row;

    printf("\n========================================\n");
    printf("           SEARCH APPOINTMENT           \n");
    printf("========================================\n");

    printf("Enter Patient ID: ");
    scanf("%d", &patient_id);

    sprintf(query,
            "SELECT appointments.id, patients.name, doctors.name, appointments.appointment_date "
            "FROM appointments "
            "JOIN patients ON appointments.patient_id = patients.id "
            "JOIN doctors ON appointments.doctor_id = doctors.id "
            "WHERE appointments.patient_id = %d", patient_id);

    if (mysql_query(conn, query) != 0)
    {
        printf("[ERROR] Search query failed: %s\n", mysql_error(conn));
        return;
    }

    result = mysql_store_result(conn);

    if (result == NULL)
    {
        printf("[ERROR] Failed to retrieve data.\n");
        return;
    }

    printf("%-8s %-20s %-20s %-15s\n", "Appt ID", "Patient Name", "Doctor Name", "Date");
    printf("-----------------------------------------------------------------------\n");

    int count = 0;
    while ((row = mysql_fetch_row(result)))
    {
        printf("%-8s %-20s %-20s %-15s\n", row[0], row[1], row[2], row[3]);
        count++;
    }

    if (count == 0)
    {
        printf("[INFO] No appointment found for Patient ID %d.\n", patient_id);
    }

    mysql_free_result(result);
}

/* ========================================================================= */
/* 5. BILLING MANAGEMENT FUNCTIONS                                           */
/* ========================================================================= */

/* Generate Bill */
void generateBill(void)
{
    int patient_id;
    float consultation, medicine, total;
    char query[400];

    printf("\n========================================\n");
    printf("             GENERATE BILL              \n");
    printf("========================================\n");

    printf("Enter Patient ID: ");
    scanf("%d", &patient_id);

    printf("Enter Consultation Fee (e.g. 500.00): ");
    scanf("%f", &consultation);

    printf("Enter Medicine Charges (e.g. 250.50): ");
    scanf("%f", &medicine);

    total = consultation + medicine;

    sprintf(query,
            "INSERT INTO bills (patient_id, consultation_fee, medicine_charge, total) "
            "VALUES (%d, %.2f, %.2f, %.2f)",
            patient_id, consultation, medicine, total);

    if (mysql_query(conn, query) == 0)
    {
        printf("\n========================================\n");
        printf("              BILL RECEIPT              \n");
        printf("========================================\n");
        printf("Patient ID       : %d\n", patient_id);
        printf("Consultation Fee : %.2f\n", consultation);
        printf("Medicine Charge  : %.2f\n", medicine);
        printf("----------------------------------------\n");
        printf("TOTAL AMOUNT     : %.2f\n", total);
        printf("========================================\n");
        printf("[SUCCESS] Bill generated and saved to database!\n");
    }
    else
    {
        printf("\n[ERROR] Failed to generate bill: %s\n", mysql_error(conn));
        printf("Note: Make sure Patient ID exists in patients table!\n");
    }
}

/* View Bills */
void viewBills(void)
{
    MYSQL_RES *result;
    MYSQL_ROW row;

    char query[] =
        "SELECT bills.id, patients.name, bills.consultation_fee, bills.medicine_charge, bills.total "
        "FROM bills "
        "JOIN patients ON bills.patient_id = patients.id";

    printf("\n================================================================================\n");
    printf("                                ALL BILLS LIST                                  \n");
    printf("================================================================================\n");

    if (mysql_query(conn, query) != 0)
    {
        printf("[ERROR] Failed to fetch bills: %s\n", mysql_error(conn));
        return;
    }

    result = mysql_store_result(conn);

    if (result == NULL)
    {
        printf("[INFO] No bills found in database.\n");
        return;
    }

    printf("%-8s %-20s %-15s %-15s %-15s\n", "Bill ID", "Patient Name", "Consultation", "Medicine", "Total");
    printf("--------------------------------------------------------------------------------\n");

    int count = 0;
    while ((row = mysql_fetch_row(result)))
    {
        printf("%-8s %-20s %-15s %-15s %-15s\n", row[0], row[1], row[2], row[3], row[4]);
        count++;
    }

    if (count == 0)
    {
        printf("[INFO] No bills generated yet.\n");
    }

    mysql_free_result(result);
}

/* ========================================================================= */
/* 6. MAIN MENU FUNCTION                                                     */
/* ========================================================================= */
int main(void)
{
    int choice;

    printf("\n==================================================\n");
    printf("         HOSPITAL MANAGEMENT SYSTEM               \n");
    printf("==================================================\n");

    /* Connect to MySQL Database */
    if (!connectDatabase())
    {
        printf("\n[FATAL ERROR] Could not connect to MySQL database.\n");
        printf("Please check your password, port number, and MySQL service status.\n");
        printf("Press Enter to exit...");
        getchar();
        return 1;
    }

    printf("\n[SUCCESS] MySQL Database Connected Successfully!\n");

    while (1)
    {
        printf("\n==================================================\n");
        printf("                    MAIN MENU                     \n");
        printf("==================================================\n");

        printf("  --- PATIENT MANAGEMENT ---\n");
        printf("  1.  Add Patient\n");
        printf("  2.  View All Patients\n");
        printf("  3.  Search Patient\n");
        printf("  4.  Update Patient\n");
        printf("  5.  Delete Patient\n");

        printf("\n  --- DOCTOR MANAGEMENT ---\n");
        printf("  6.  Add Doctor\n");
        printf("  7.  View All Doctors\n");
        printf("  8.  Search Doctor\n");

        printf("\n  --- APPOINTMENT MANAGEMENT ---\n");
        printf("  9.  Book Appointment\n");
        printf("  10. View Appointments\n");
        printf("  11. Search Appointment\n");

        printf("\n  --- BILL MANAGEMENT ---\n");
        printf("  12. Generate Bill\n");
        printf("  13. View Bills\n");

        printf("\n  ---------------------------\n");
        printf("  14. Exit Program\n");
        printf("==================================================\n");

        printf("Enter your choice (1-14): ");
        if (scanf("%d", &choice) != 1)
        {
            printf("\n[ERROR] Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); /* Clear input buffer */
            continue;
        }

        switch (choice)
        {
            case 1:
                addPatient();
                break;
            case 2:
                viewPatients();
                break;
            case 3:
                searchPatient();
                break;
            case 4:
                updatePatient();
                break;
            case 5:
                deletePatient();
                break;
            case 6:
                addDoctor();
                break;
            case 7:
                viewDoctors();
                break;
            case 8:
                searchDoctor();
                break;
            case 9:
                bookAppointment();
                break;
            case 10:
                viewAppointments();
                break;
            case 11:
                searchAppointment();
                break;
            case 12:
                generateBill();
                break;
            case 13:
                viewBills();
                break;
            case 14:
                printf("\nThank you for using Hospital Management System!\n");
                mysql_close(conn);
                return 0;
            default:
                printf("\n[ERROR] Invalid choice! Please select an option between 1 and 14.\n");
        }
    }

    return 0;
}
