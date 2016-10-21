/***********************   include files   ******************************************************/
#include <ruby.h>
#include <astrocon.h>
#include "stdio.h"
#include "stdlib.h"
#include <math.h>
#include <string.h>
#include <stdbool.h>

/***********************   macro definitions   **************************************************/

#define SUN 0
#define MOON 1
#define RA 23
#define DEC 33
#define DATA_NAME       "USA"
#define CLUB_NAME       "club"
#define CLUB_EXT        ".txt"
#define DATA_EXT        ".bin"
#define DST_NAME        "USA"
#define DST_EXT         ".txt"
#define DATA_PATH       "./ext/solunar/Data_Files/"
#define CLUB_PATH       "./ext/solunar/Club_Files/"
#define DST_PATH        "./ext/solunar/DST_Files/"
#define SUN_FILE        "./ext/solunar/Source_Files/sun.txt"
#define MOON_FILE       "./ext/solunar/Source_Files/moon.txt"
#define PHASE_FILE      "./ext/solunar/Source_Files/phase.txt"
#define ILLUM_16_FILE   "./ext/solunar/Source_Files/ilum_2016.txt"
#define ILLUM_17_FILE   "./ext/solunar/Source_Files/ilum_2017.txt"
#define ILLUM_18_FILE   "./ext/solunar/Source_Files/ilum_2018.txt"
#define ILLUM_19_FILE   "./ext/solunar/Source_Files/ilum_2019.txt"
#define ILLUM_20_FILE   "./ext/solunar/Source_Files/ilum_2020.txt"
#define ILLUM_21_FILE   "./ext/solunar/Source_Files/ilum_2021.txt"
#define ILLUM_22_FILE   "./ext/solunar/Source_Files/ilum_2022.txt"
#define ILLUM_23_FILE   "./ext/solunar/Source_Files/ilum_2023.txt"
#define ILLUM_24_FILE   "./ext/solunar/Source_Files/ilum_2024.txt"
#define ILLUM_25_FILE   "./ext/solunar/Source_Files/ilum_2025.txt"
#define ILLUM_26_FILE   "./ext/solunar/Source_Files/ilum_2026.txt"

#define CLUB_FAIL       1
#define DATA_FAIL       2       
#define SUN_FAIL        3       
#define MOON_FAIL       4   
#define DTS_FAIL        5       
#define PHASE_FAIL      6   
#define ILLUM_16_FAIL   7
#define ILLUM_17_FAIL   8
#define ILLUM_18_FAIL   9
#define ILLUM_19_FAIL   10
#define ILLUM_20_FAIL   11
#define ILLUM_21_FAIL   12
#define ILLUM_22_FAIL   13
#define ILLUM_23_FAIL   14
#define ILLUM_24_FAIL   15
#define ILLUM_25_FAIL   16
#define ILLUM_26_FAIL   17
#define ARG_ERROR       99
//file information
#define DATA_FILE_SIZE  4018
#define JPL_DATE_SIZE   12
#define PHASE_FILE_SIZE 544
#define DST_FILE_SIZE   11
#define RST_MAX_TRIES   50
#define JDATE_BASE      2457388.5   // January 1, 2016
#define YES             1
#define NO              0
#define MINUTES_PER_DAY (24 * 60)
//exceptions
#define NEXT_DAY    1
#define PREV_DAY    2   
#define NONE_TODAY  3
#define ROUTINE     4
#define NEVER_RISE  5
#define NEVER_SET   6
#define ERROR       7
#define RST_ERROR   8
#define RST_FAIL    9
//phases
#define NEW_MOON    1
#define WAX_CRES    2
#define FIRST_QTR   3
#define WAX_GIBB    4
#define FULL_MOON   5
#define WAN_GIBB    6
#define LAST_QTR    7
#define WAN_CRES    8

/***********************   typedefs   ***********************************************************/

typedef struct {
    double ra;
    double dec;
} jpl_data;
typedef struct {
    char date[JPL_DATE_SIZE];
    jpl_data sun;
    jpl_data moon;
    bool dst;
    int phase;
    int phase_time;
    int illum;
} jpl_type;
typedef struct {
    int minute;
    int second;
    int exception;
} time_type;
typedef struct {
    time_type ris;
    time_type set;
    time_type trn;
    time_type tru;
} rst_type;
typedef struct {
    char date[JPL_DATE_SIZE];
    int gmt_offset;
    rst_type sun;
    rst_type moon;
    int moon_phase;
    int phase_time;
    int moon_illum;
} sol_type;
typedef struct {
    double start;
    double stop;
} dst_type;

/***********************   constant arrays   ****************************************************/

const int year_days[11] = {
    0,      // 2016
    366,    // 2017
    731,    // 2018
    1096,   // 2019
    1461,   // 2020
    1827,   // 2021
    2192,   // 2022
    2557,   // 2023
    2922,   // 2024
    3288,   // 2025
    3653,   // 2026
};
const int month_days[12] = {
    0,      // JAN
    31,     // FEB
    59,     // MAR
    90,     // APR
    120,    // MAY
    151,    // JUN
    181,    // JUL
    212,    // AUG
    243,    // SEP
    273,    // OCT
    304,    // NOV
    334,    // DEC
};
const int leap_days[12] = {
    0,      // JAN
    31,     // FEB
    60,     // MAR
    91,     // APR
    121,    // MAY
    152,    // JUN
    182,    // JUL
    213,    // AUG
    244,    // SEP
    274,    // OCT
    305,    // NOV
    335,    // DEC
};
char error_msg[][36] = {
    "Operation Successful",
    "Cannot open Club file",
    "Cannot open Data file",
    "Cannot open Sun file",
    "Cannot open Moon file",
    "Cannot open DST file",
    "Cannot open Phase file",
    "Cannot open 2016 Illumination file",
    "Cannot open 2017 Illumination file",
    "Cannot open 2018 Illumination file",
    "Cannot open 2019 Illumination file",
    "Cannot open 2020 Illumination file",
    "Cannot open 2021 Illumination file",
    "Cannot open 2022 Illumination file",
    "Cannot open 2023 Illumination file",
    "Cannot open 2024 Illumination file",
    "Cannot open 2025 Illumination file",
    "Cannot open 2026 Illumination file"
};

/***********************   function prototypes   ************************************************/

int         TestBuildDataFile(void);
int         BuildDataFile(char*);
int         TestSummary(void);
int         Summary(char*, double, double, int, int, int, char*);
int         TestClubFile(void);
char*       ClubFile(char*, char*, int, double, double, int, int, int, char*, char*);
double      ConvertDate(char*);
int         GetSunData(void);
int         GetMoonData(void);
int         GetDateData(void);
int         GetDstData(char*);
int         GetIllumData(void);
int         GetPhaseData(void);
int         SaveDataFile(char*);
int         Solunar(sol_type*, double, double, double, int, int, char*);
int         GetIllumYear(const char*, int*);
time_type   FmtTime(double);
void        FmtTimeStr(char*, time_type, int);
time_type   AdjustTimes(time_type, time_type, time_type, int);
// original Heafner functions with some changes
rst_type    RST(FILE*, int, double, double, double);
static int          RST_Interpolate(int, double, double, double, double, double*, double*, double,
                            double, double, double, double, double, double, double, double*);
double      deg(double x);
void        GetGST(double, int, double*);
double      amodulo(double, double);

/***********************   variable delarations   ***********************************************/

jpl_type jpl_temp[DATA_FILE_SIZE];
dst_type dst[DST_FILE_SIZE];

/***********************   mode selection   *****************************************************/
/*
Uuncomment one of these to control whether the executable will enter the Console Mode, to display
a one cay summary, the Club Mode, to porepare the Club File, or the build Mode, to build a new
data file from a revised DST file.                                                              */

//define CONSOLE_MODE
#define CLUB_MODE
//#define BUILD_MODE

/***********************   Main

This is the entry point for the program.

RETURN VALUE:   EXIT_SUCCESS or Error Code

PARAMETERS:     Varies depending on mode
*/
/*#ifdef CONSOLE_MODE
int main()
{
    enum { BUILD = 'B', CLUB = 'C', DISPLAY = 'D' };
    int task;
    char line[100];

    printf("Enter: Club File, Display Summary, Build Data File: ");
    fgets(line, sizeof line, stdin);
    if (line[0] == '\n')    strcpy(line, "Club");
    task = line[0];
    switch (task)
    {
    case BUILD:
    case BUILD + 32:
        printf("\nBUILD DATA FILE\n\n");
        TestBuildDataFile();
        break;
    case DISPLAY:
    case DISPLAY + 32:
        printf("\nDISPLAY SUMMARY\n\n");
        TestSummary();
        break;
    default:
        printf("\nCLUB FILE\n\n");
        TestClubFile();
        break;
    }
    return 0;
}
#endif*/

static VALUE generate(VALUE self, VALUE r_date_str, VALUE r_count, VALUE r_lat,
    VALUE r_lon, VALUE r_gmt_offset, VALUE r_dst_offset, VALUE r_military_time)
{
    char* result;
    int gmt_offset;
    int dst_time;
    int am_pm;
    int count;
    double lat;
    double lon;
    char *club_name;
    char *date_str;
    char *data_name;
    VALUE ret_v;

    club_name = "club"; //Constant, as this isn't really used for anything
    date_str = RSTRING_PTR(r_date_str);
    count = NUM2INT(r_count);
    lat = NUM2DBL(r_lat);
    lon = NUM2DBL(r_lon);
    gmt_offset = NUM2INT(r_gmt_offset);
    dst_time = NUM2INT(r_dst_offset);
    am_pm = NUM2INT(r_military_time);
    data_name = "USA"; //Constant, for now
    int outputLength;
    outputLength = count*251;
    char output[outputLength];
    result = ClubFile(  club_name, date_str, count, lat, lon, 
                        gmt_offset, dst_time, am_pm, data_name, output);
    ret_v = rb_str_new2(result);

    return ret_v;
}

/*#ifdef BUILD_MODE
int main(int argc, char *argv[])
{
    return argc < 2? ARG_ERROR : BuildDataFile(argv[1]);

}
#endif*/

/***********************   Build Data File Test

This function exercises BuildDataFile(). It is used for testing.

RETURN VALUE:   EXIT_SUCCESS or Error Code

PARAMETERS:     none
*/
int TestBuildDataFile()
{
    int success;
    char line[100];
    char file_name[100];

    printf("Enter DST/Data File Name: ");
    fgets(line, sizeof line, stdin);
    if (line[0] == '\n')    strcpy(file_name, DST_NAME);
    else
    {
        line[strlen(line) - 1] = '\0';
        strcpy(file_name, line);
    }
    success = BuildDataFile(file_name);
    printf("\n%s", error_msg[success]);
    printf("\n\nOK? ");
    fgets(line, sizeof line, stdin);
    return success;
}
/***********************   Build Data File

This function prepares a the Data file from a number of source data files. If one of the files
cannot be opened, the program stops there and returns a number indicating the failure.

RETURN VALUE:   EXIT_SUCCESS or Error Code

PARAMETERS:     name of DST file and resultant Data File

*/
int BuildDataFile(char *file_name)
{
    int success;
    char data_filename[32];
    char dst_filename[32];

    strcpy(data_filename, DATA_PATH);
    strcat(data_filename, file_name);
    strcat(data_filename, DATA_EXT);
    strcpy(dst_filename, DST_PATH);
    strcat(dst_filename, file_name);
    strcat(dst_filename, DST_EXT);

    success = 0;
    if (success == EXIT_SUCCESS) success = GetSunData();
    if (success == EXIT_SUCCESS) success = GetMoonData();
    if (success == EXIT_SUCCESS) success = GetDateData();
    if (success == EXIT_SUCCESS) success = GetDstData(dst_filename);
    if (success == EXIT_SUCCESS) success = GetIllumData();
    if (success == EXIT_SUCCESS) success = GetPhaseData();
    if (success == EXIT_SUCCESS) success = SaveDataFile(data_filename);
    return success;
}

/***********************   Display Summary Test

This function exercises Summary(). It is used for testing.

RETURN VALUE:   EXIT_SUCCESS or Error Code

PARAMETERS:     none
*/
int TestSummary(void)
{
    int dst_time;
    int am_pm;
    char date_str[100];
    char file_name[100];
    char line[1024];
    double lat;
    double lon;
    int gmt_offset;
    int success;

    printf("Enter Data File name: ");
    fgets(line, sizeof line, stdin);
    if (line[0] == '\n')    strcpy(file_name, DATA_NAME);
    else
    {
        line[strlen(line) - 1] = '\0';
        strcpy(file_name, line);
    }

    printf("Enter latitude (+N/-S, dd.dddd): ");
    fgets(line, sizeof line, stdin);
    if (line[0] == '\n')    lat = 34.5082;
    else                    sscanf(line, "%lf", &lat);

    printf("Enter longitude (+W/-E, dd.dddd): ");
    fgets(line, sizeof line, stdin);
    if (line[0] == '\n')    lon = 82.6500;
    else                    sscanf(line, "%lf", &lon);

    printf("Enter offset minutes from GMT (+W/-E): ");
    fgets(line, sizeof line, stdin);
    if (line[0] == '\n')    gmt_offset = 5 * 60;
    else                    sscanf(line, "%i", &gmt_offset);

    printf("Enter DST time in minutes: ");
    fgets(line, sizeof line, stdin);
    if (line[0] == '\n')    dst_time = 60;
    else                    sscanf(line, "%i", &dst_time);

    printf("Enter Date - YYYY/MM/DD: ");
    fgets(line, sizeof line, stdin);
    if (line[0] == '\n')    strcpy(date_str, "2016/07/04");
    else
    {
        line[strlen(line) - 1] = '\0';
        strcpy(date_str, line);
    }
    printf("24 Hour Format (YES/NO): ");
    fgets(line, sizeof line, stdin);
    am_pm = ((line[0] == 'Y') || (line[0] == 'y')) ? NO : YES;

    success = Summary(date_str, lat, lon, gmt_offset, dst_time, am_pm, file_name);

    printf("\n%s", error_msg[success]);

    printf("\n\nOK? ");

    fgets(line, sizeof line, stdin);
    return success;
}
/***********************   Display Summary

This function displays all data for the parameters specified.

RETURN VALUE:   EXIT_SUCCESS or Error Code

PARAMETERS:     date (string)
                latitude (+ is north)
                longitude (+ is west)
                offset from GMT in minutes (+ is west)
                daylight savings time change in minutes
                time format desired
                name of the data file
*/
int Summary(char *date_str, double lat, double lon, int gmt_offset, int dst_time,
            int am_pm, char *data_file)
{
    sol_type solunar;
    time_type temp;
    double jdate;
    int n;
    int success;
    char s_ris[10];
    char s_trn[10];
    char s_set[10];
    char s_tru[10];
    char m_ris[10];
    char m_trn[10];
    char m_set[10];
    char m_tru[10];
    char p_tim[10];
    char filename[100];
    const char phase[9][16] =
    {   "*** ERROR ***  ",
        "NEW_MOON       ",
        "Waxing Crescent",
        "FIRST QUARTER  ",
        "Waxing Gibbous ",
        "FULL MOON      ",
        "Waning Gibbous ",
        "LAST QUARTER   ",
        "Waning Cescent " };

    lat *= D2R;
    lon *= D2R;

    strcpy(filename, DATA_PATH);
    strcat(filename, data_file);
    strcat(filename, DATA_EXT);
    jdate = ConvertDate(date_str);

    success = Solunar(&solunar, jdate, lat, lon, gmt_offset, dst_time, filename);
    if (success == EXIT_SUCCESS)
    {
        FmtTimeStr(s_ris, solunar.sun.ris, am_pm);
        FmtTimeStr(s_trn, solunar.sun.trn, am_pm);
        FmtTimeStr(s_set, solunar.sun.set, am_pm);
        FmtTimeStr(s_tru, solunar.sun.tru, am_pm);
        FmtTimeStr(m_ris, solunar.moon.ris, am_pm);
        FmtTimeStr(m_trn, solunar.moon.trn, am_pm);
        FmtTimeStr(m_set, solunar.moon.set, am_pm);
        FmtTimeStr(m_tru, solunar.moon.tru, am_pm);

        printf("\n%s  SUN    local times (GMT - %i)\n", solunar.date, solunar.gmt_offset);
        printf("   Rise =          %s\n", s_ris);
        printf("   Transit =       %s\n", s_trn);
        printf("   Set =           %s\n", s_set);
        printf("   Tran Under =    %s\n", s_tru);
        printf("\n%s  MOON   local times (GMT - %i)\n", solunar.date, solunar.gmt_offset);
        printf("   Rise =          %s\n", m_ris);
        printf("   Transit =       %s\n", m_trn);
        printf("   Set =           %s\n", m_set);
        printf("   Tran Under =    %s\n", m_tru);

        for (n = 0; n < 8; n++)
        {
            p_tim[n] = ' ';
        }
        p_tim[n] = '\0';
        if (solunar.phase_time != 0)
        {
            temp.minute = solunar.phase_time;
            temp.second = 0;
            temp.exception = 0;
            FmtTimeStr(p_tim, temp, am_pm);
        }
        printf( "   %s %s\n   %d%% Illuminated\n",
                phase[solunar.moon_phase], p_tim, solunar.moon_illum);
    }
    return success;
}

/***********************   Club File Test

This function exercises ClubFile(). It is used for testing.

RETURN VALUE:   EXIT_SUCCESS or Error Code

PARAMETERS:     none
*/
int TestClubFile(void)
{
    int success;
    int dst_time;
    int am_pm;
    char club_name[100];
    char data_name[100];
    char start_date[100];
    char line[1024];
    double lat;
    double lon;
    int count;
    int gmt_offset;

    printf("Enter Club Name: ");
    fgets(line, sizeof line, stdin);
    if (line[0] == '\n')    strcpy(club_name, CLUB_NAME);
    else
    {
        line[strlen(line) - 1] = '\0';
        strcpy(club_name, line);
    }
    printf("Enter Data File Name: ");
    fgets(line, sizeof line, stdin);
    if (line[0] == '\n')    strcpy(data_name, DATA_NAME);
    else
    {
        line[strlen(line) - 1] = '\0';
        strcpy(data_name, line);
    }
    printf("Enter latitude (+N/-S, dd.dddd): ");
    fgets(line, sizeof line, stdin);
    if (line[0] == '\n')    lat = 34.5082;
    else                    sscanf(line, "%lf", &lat);

    printf("Enter longitude (+W/-E, dd.dddd): ");
    fgets(line, sizeof line, stdin);
    if (line[0] == '\n')    lon = 82.6500;
    else                    sscanf(line, "%lf", &lon);

    printf("Enter offset minutes from GMT (+W/-E): ");
    fgets(line, sizeof line, stdin);
    if (line[0] == '\n')    gmt_offset = 5 * 60;
    else                    sscanf(line, "%i", &gmt_offset);

    printf("Enter DST time in minutes: ");
    fgets(line, sizeof line, stdin);
    if (line[0] == '\n')    dst_time = 60;
    else                    sscanf(line, "%i", &dst_time);

    printf("Enter Starting Date - YYYY/MM/DD: ");
    fgets(line, sizeof line, stdin);
    if (line[0] == '\n')    strcpy(start_date, "2016/07/04");
    else
    {
        line[strlen(line) - 1] = '\0';
        strcpy(start_date, line);
    }

    printf("Enter number of days to be listed: ");
    fgets(line, sizeof line, stdin);
    if (line[0] == '\n')    count = 1;
    else                    sscanf(line, "%i", &count);

    printf("24 Hour Format (YES/NO): ");
    fgets(line, sizeof line, stdin);
    am_pm = ((line[0] == 'Y') || (line[0] == 'y')) ? NO : YES;

    //success = ClubFile(club_name, start_date, count, lat, lon, gmt_offset, dst_time, am_pm, data_name);

    printf("\n%s", error_msg[success]);
    printf("\n\nOK? ");
    fgets(line, sizeof line, stdin);

    return success;
}
/***********************   Prepare Club Data File

This function prepares a text file with a list of values for the date range specified. 

RETURN VALUE:   EXIT_SUCCESS or Error Code

PARAMETERS:     name of Club File
                starting date (string)
                number of days to be listed
                latitude (+ is north)
                longitude (+ is weast)
                offset from GMT in minutes (+ is west)
                daylight savings time change in minutes
                name of Data File
*/
char* ClubFile(char *club_name, char *start_date, int count, double lat, double lon,
                int gmt_offset, int dst_time, int am_pm, char *data_name, char *output)
{
    FILE *file;
    int i;
    int n;
    int success;
    double jdate;
    time_type temp;
    sol_type solunar;
    char s_ris[10];
    char s_trn[10];
    char s_set[10];
    char s_tru[10];
    char m_ris[10];
    char m_trn[10];
    char m_set[10];
    char m_tru[10];
    char p_tim[10];
    char data_filename[100];
    char club_filename[100];
    const char phase[9][16] =
    { "*** ERROR ***  ",
        "New Moon       ",
        "Waxing Crescent",
        "First Quarter  ",
        "Waxing Gibbous ",
        "Full Moon      ",
        "Waning Gibbous ",
        "Last Quarter   ",
        "Waning Crescent" };

    lat *= D2R;
    lon *= D2R;

    strcpy(club_filename, CLUB_PATH);
    strcat(club_filename, club_name);
    strcat(club_filename, CLUB_EXT);
    strcpy(data_filename, DATA_PATH);
    strcat(data_filename, data_name);
    strcat(data_filename, DATA_EXT);
    jdate = ConvertDate(start_date);

    for (i = 0; i < count; i++)
    {
        success = Solunar(&solunar, jdate + i, lat, lon, gmt_offset, dst_time, data_filename);
        if (success == EXIT_SUCCESS)
        {
            FmtTimeStr(s_ris, solunar.sun.ris, am_pm);
            FmtTimeStr(s_trn, solunar.sun.trn, am_pm);
            FmtTimeStr(s_set, solunar.sun.set, am_pm);
            FmtTimeStr(s_tru, solunar.sun.tru, am_pm);
            FmtTimeStr(m_ris, solunar.moon.ris, am_pm);
            FmtTimeStr(m_trn, solunar.moon.trn, am_pm);
            FmtTimeStr(m_set, solunar.moon.set, am_pm);
            FmtTimeStr(m_tru, solunar.moon.tru, am_pm);

            for (n = 0; n < 8; n ++)
            {
                p_tim[n] = ' ';
            }
            p_tim[n] = '\0';
            if (solunar.phase_time != 0)
            {
                temp.minute = solunar.phase_time;
                temp.second = 0;
                temp.exception = 0;

                FmtTimeStr(p_tim, temp, am_pm);
            }
            char charout[250];
            sprintf(charout,"%s, %s, %s, %s, %s, %s, %s, %s, %s, %i, %s, %s, %i%%\n",
                solunar.date, s_ris, s_trn, s_set, s_tru, m_ris, m_trn, m_set, m_tru,
                solunar.gmt_offset, phase[solunar.moon_phase], p_tim, solunar.moon_illum);
            if(strlen(output) == 0){
                strcpy(output,charout);
            }
            else{
                strcat(output,";");
                strcat(output,charout);
            }
        }
    }

    return output;

}
/***********************   Convert Date

This function returns the jdate from a string containing the desired date in a YYYY-MM-DD format.

RETURN VALUE:   jdate
                0.0 if text date is not valid

PARAMETERS:     pointer to date string (yyyy/mm/dd
*/
double ConvertDate(char* string)
{
    int m[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int l[] = {31,29,31,30,31,30,31,31,30,31,30,31};
    int year;
    int month;
    int day;
    int error;
    double result;
    
    error = NO;
    year = atoi(string);
    month = atoi(string + 5);
    day = atoi(string + 8);
    if ((year < 2016)   ||  (year > 2026))                              error = YES;
    if ((month < 1)     ||  (month > 12))                               error = YES;
    if ((day < 1)       ||  ((year % 4 == 0) && (day > l[month - 1])))  error = YES;
    if ((day < 1)       ||  ((year % 4 != 0) && (day > m[month - 1])))  error = YES;
    if (error == NO)
    {
        result =  JDATE_BASE;
        result += year_days[year - 2016];
        result += year % 4 == 0? leap_days[month - 1]: month_days[month - 1];
        result += day - 1;
    }
    else
    {
        result = 0.0;
    }
    return result;
}
/***********************   Get Sun Data

This function gets the sun's JPL values for right ascension and declination values from the JPL
data in sun.txt and puts them into jpl_temp[].

RETURN VALUE:   EXIT_SUCCESS or Error Code

PARAMETERS:     none
*/
int GetSunData(void)
{
    FILE *file;
    int n;
    int success;
    char buffer[100];

    file = fopen(SUN_FILE, "r");
    if (file != NULL)
    {
        for (n = 0; n < DATA_FILE_SIZE; n++)
        {
            fgets(buffer, 100, file);
            jpl_temp[n].sun.ra = atof(buffer + RA);
            jpl_temp[n].sun.dec = atof(buffer + DEC);
        }
        fclose(file);
        success = EXIT_SUCCESS;
    }
    else
    {
        success = SUN_FAIL;
    }
    return success;
}

/***********************   Get Moon Data

This function gets the moon's JPL values for right ascension and declination values from the JPL
data in sun.txt and puts them into jpl_temp[].

RETURN VALUE:   EXIT_SUCCESS or Error Code

PARAMETERS:     none
*/
int GetMoonData(void)
{
    FILE* file;
    int n;
    int success;
    char buffer[100];

    file = fopen(MOON_FILE, "r");
    if (file != NULL)
    {
        for (n = 0; n < DATA_FILE_SIZE; n++)
        {
            fgets(buffer, 100, file);
            jpl_temp[n].moon.ra = atof(buffer + RA);
            jpl_temp[n].moon.dec = atof(buffer + DEC);
        }
        fclose(file);
        success = EXIT_SUCCESS;
    }
    else
    {
        success = MOON_FAIL;
    }
    return success;
}

/***********************   Get Date String

This function gets the date string from the JPL data in moon.txt and puts it into jpl_temp[].
This operation could have been done as part of GetMoonData() or GetSunData(), but is separated
for clarity.

RETURN VALUE:   EXIT_SUCCESS or Error Code

PARAMETERS:     none
*/
int GetDateData(void)
{
    FILE* file;
    int n;
    int i;
    int success;
    char buffer[100];

    file = fopen(MOON_FILE, "r");
    if (file != NULL)
    {
        for (n = 0; n < DATA_FILE_SIZE; n++)
        {
            fgets(buffer, 100, file);
            for (i = 0; i < JPL_DATE_SIZE - 1; i++)
            {
                jpl_temp[n].date[i] = buffer[i + 1];
            }
            jpl_temp[n].date[i] = '\0';
        }
        fclose(file);
        success = EXIT_SUCCESS;
    }
    else
    {
        success = MOON_FAIL;
    }
    return success;
}

/***********************   Get Daylight Savings Time Data

For every element of the jpl_temp[] array, this function determines whether or not the associated
date is subject to daylight savinhgs time and the dst member is updated.

RETURN VALUE:   EXIT_SUCCESS or Error Code

PARAMETERS:     none
*/
int GetDstData(char *filename)
{
    FILE* file;
    char buffer[100];
    int n;
    int y;
    int success;
    double jdate;

    file = fopen(filename, "r");
    if (file != NULL)
    {
        for (y = 0; y < DST_FILE_SIZE; y++)
        {
            fgets(buffer, 100, file);
            dst[y].start = ConvertDate(buffer);
            dst[y].stop = ConvertDate(buffer + 12);
        }
        for (n = 0; n < DATA_FILE_SIZE; n++)
        {
            jdate = JDATE_BASE + n;
            jpl_temp[n].dst = NO;
            for (y = 0; y < (sizeof dst / sizeof(dst_type)); y++)
            {
                if (jdate < dst[y].stop)
                {
                    if (jdate >= dst[y].start)
                    {
                        jpl_temp[n].dst = YES;
                    }
                    break;
                }
            }
        }
        fclose(file);
        success = EXIT_SUCCESS;
    }
    else
    {
        success = DTS_FAIL;
    }
    return success;
}
/***********************   Get Moon Phase Data

This function gets the moon's phase data from the phase.txt file and puts it into jpl_temp[].

RETURN VALUE:   EXIT_SUCCESS or Error Code

PARAMETERS:     none
*/
int GetPhaseData(void)
{
    FILE* file;
    bool match;
    int c;
    int j;
    int i;
    int n;
    int phase;
    int success;
    char date[11];
    int minute;
    char buffer[100];

    j = 0;
    for (n = 0; n < DATA_FILE_SIZE; n++)
    {
        jpl_temp[n].phase = 0;
        jpl_temp[n].phase_time = 0;
    }
    file = fopen(PHASE_FILE, "r");
    if (file != NULL)
    {
        for (n = 0; n < PHASE_FILE_SIZE; n++)
        {
            fgets(buffer, 100, file);
            i = 0;
            while (buffer[i++] != '\t');
            for (c = 0; c < 11; c++)
            {
                date[c] = buffer[i] == ' ' ? '-' : buffer[i];
                i++;
            }
            i ++;
            minute  = atoi(buffer + i) * 60;
            minute += atoi(buffer + i + 3);
            if (minute == 0)
            {
                minute = 24 * 60;
            }
            match = NO;
            while (match == NO)
            {
                for (c = 0; c < 11; c++)
                {
                    if (date[c] != jpl_temp[j].date[c])
                    {
                        break;
                    }
                }
                if (c == 11)
                {
                    match = YES;
                }
                else
                {
                    j++;
                }
            }
            if      (buffer[2] == 'w')  jpl_temp[j].phase = NEW_MOON;
            else if (buffer[2] == 'r')  jpl_temp[j].phase = FIRST_QTR;
            else if (buffer[2] == 'l')  jpl_temp[j].phase = FULL_MOON;
            else                        jpl_temp[j].phase = LAST_QTR;
            jpl_temp[j].phase_time = minute;
        }
        fclose(file);
        success = EXIT_SUCCESS;

        // for each element of the jpl array:
        //      record the phase based on the last major phases
        //      record the the time of the last major phase
        phase = FULL_MOON;              // 12/25/2015 phase 
        minute = (11 * 60)+ 11;         // 12/25/2015 Full Moon time
        for (n = 0; n < DATA_FILE_SIZE; n++)
        {
            if (jpl_temp[n].phase == 0)
            {
                jpl_temp[n].phase = phase + 1;
            }
            else
            {
                phase = jpl_temp[n].phase;
            }
        }
    }
    else
    {
        success = PHASE_FAIL;
    }
    return success;
}

/***********************   Get Moon Illumination Percentage Data

This function gets the moon's percent illumination data from Ilum 20xx.txt files and puts it
into jpl_temp[].

RETURN VALUE:   EXIT_SUCCESS or Error Code

PARAMETERS:     none
*/
int GetIllumData(void)
{
    int index;
    int success;

    index = 0;
    success = EXIT_SUCCESS;
    if (success == EXIT_SUCCESS) success = GetIllumYear(ILLUM_16_FILE, &index);
    if (success == EXIT_SUCCESS) success = GetIllumYear(ILLUM_17_FILE, &index);
    if (success == EXIT_SUCCESS) success = GetIllumYear(ILLUM_18_FILE, &index);
    if (success == EXIT_SUCCESS) success = GetIllumYear(ILLUM_19_FILE, &index);
    if (success == EXIT_SUCCESS) success = GetIllumYear(ILLUM_20_FILE, &index);
    if (success == EXIT_SUCCESS) success = GetIllumYear(ILLUM_21_FILE, &index);
    if (success == EXIT_SUCCESS) success = GetIllumYear(ILLUM_22_FILE, &index);
    if (success == EXIT_SUCCESS) success = GetIllumYear(ILLUM_23_FILE, &index);
    if (success == EXIT_SUCCESS) success = GetIllumYear(ILLUM_24_FILE, &index);
    if (success == EXIT_SUCCESS) success = GetIllumYear(ILLUM_25_FILE, &index);
    if (success == EXIT_SUCCESS) success = GetIllumYear(ILLUM_26_FILE, &index);
    return success;
}

/***********************   Save Data File

After the daylight savings data, date string and the right ascension and declination values for
the sun and moon plus mone phase and illumination have been placed in jpl.temp[], this function
writes the jpl data to the Data File.

RETURN VALUE:   EXIT_SUCCESS or Error Code

PARAMETERS:     filepath of Data File
*/
int SaveDataFile(char *filename)
{
    FILE *file;
    int success;

    file = fopen(filename, "wb");
    if (file != NULL)
    {
        fwrite(jpl_temp, sizeof(jpl_type), DATA_FILE_SIZE, file);
        fclose(file);
        success = EXIT_SUCCESS;
    }
    else
    {
        success = DATA_FAIL;
    }
    return success;
}

/***********************   Solunar

This function returns all the informatin required by the Knockdown program.

As to the phases of the moon:

The JPL file stores all data by GMT date. Unless the GMT offset time is 0, the local date
overlaps parts of two GMT dates:

East of Greenwich, the local date overlaps that part of the previous GMT date where the
GMT time is within [gmt_offset] minutes before midnight; the local date overlaps the current
GMT date the rest of the day.

West of Greenwich, the local date overlaps that part of the following GMT date where the
GMT time is within [gmt_offset] minutes after midnight; the local date overlaps the current 
GMT date the rest of the day.

The Primary moon phases are reported in local time and date. Use the GMT date where the
time of the moon phase overlaps the current local date.


RETURN VALUE:   EXIT_SUCCESS or Error Code

PARAMETERS:     sol_type structure with sun, moon, date, and DST adjusted GME offset information
                julian date
                latitude (+ is north)
                longitude (+ is west)
                minutes easr or west of GMT (+ is west)
                DST time in minutes
                filepath of Data File
*/
int Solunar(sol_type *result, double jdate, double lat, double lon,
            int gmt_offset, int dst_time, char *filename)
{
    enum {p_day, c_day, f_day};
    int n;
    int index;
    int success;
    FILE *file;
    jpl_type jpl[3];
    rst_type times_p;
    rst_type times;
    rst_type times_f;
    int phase_time;
    int months[13][5] = {
        { 'J', 'a', 'n', '0', '1' },
        { 'F', 'e', 'b', '0', '2' },
        { 'M', 'a', 'r', '0', '3' },
        { 'A', 'p', 'r', '0', '4' },
        { 'M', 'a', 'y', '0', '5' },
        { 'J', 'u', 'n', '0', '6' },
        { 'J', 'u', 'l', '0', '7' },
        { 'A', 'u', 'g', '0', '8' },
        { 'S', 'e', 'p', '0', '9' },
        { 'O', 'c', 't', '1', '0' },
        { 'N', 'o', 'v', '1', '1' },
        { 'D', 'e', 'c', '1', '2' },
        { 0,   0,   0,   '?', '?' } };

    //lon = -lon;

    // get date
    index = (int)(floor(jdate) - floor(JDATE_BASE));
    file = fopen(filename, "rb");
    if (file != NULL)
    {
        fseek(file, ((index - 1) * sizeof(jpl_type)), SEEK_SET);
        fread(&jpl[0], sizeof(jpl_type), 3, file);
        for (n = 0; n < 12; n++)
        {
            if (    (jpl[c_day].date[5] == months[n][0])
                &&  (jpl[c_day].date[6] == months[n][1])
                &&  (jpl[c_day].date[7] == months[n][2]) )
            {
                break;
            }
        }
        result->date[0] = jpl[c_day].date[0];
        result->date[1] = jpl[c_day].date[1];
        result->date[2] = jpl[c_day].date[2];
        result->date[3] = jpl[c_day].date[3];
        result->date[4] = '-';
        result->date[5] = months[n][3];
        result->date[6] = months[n][4];
        result->date[7] = '-';
        result->date[8] = jpl[c_day].date[9];
        result->date[9] = jpl[c_day].date[10];
        result->date[10] = '\0';

        // adjust gmt offset for dst
        if ((dst_time != 0) && (jpl[c_day].dst == YES))
        {
            gmt_offset -= dst_time;
        }
        result->gmt_offset = gmt_offset;

        // get values for the sun
        times_p = RST(file, SUN, jdate - 1, lat, lon);
        times   = RST(file, SUN, jdate, lat, lon);
        times_f = RST(file, SUN, jdate + 1, lat, lon);
        result->sun.ris = AdjustTimes(times_p.ris, times.ris, times_f.ris, gmt_offset);
        result->sun.trn = AdjustTimes(times_p.trn, times.trn, times_f.trn, gmt_offset);
        result->sun.set = AdjustTimes(times_p.set, times.set, times_f.set, gmt_offset);

        // get values for the moon  
        times_p = RST(file, MOON, jdate - 1, lat, lon);
        times   = RST(file, MOON, jdate, lat, lon);
        times_f = RST(file, MOON, jdate + 1, lat, lon);
        result->moon.ris = AdjustTimes(times_p.ris, times.ris, times_f.ris, gmt_offset);
        result->moon.trn = AdjustTimes(times_p.trn, times.trn, times_f.trn, gmt_offset);
        result->moon.set = AdjustTimes(times_p.set, times.set, times_f.set, gmt_offset);

        // get value for sun and moon transit underfoot ...
        if (lon < 0)    lon += PI;
        else            lon -= PI;

        // ... sun
        times_p = RST(file, SUN, jdate - 1, lat, lon);
        times   = RST(file, SUN, jdate, lat, lon);
        times_f = RST(file, SUN, jdate + 1, lat, lon);
        result->sun.tru = AdjustTimes(times_p.trn, times.trn, times_f.trn, gmt_offset);

        // ... moon
        times_p = RST(file, MOON, jdate - 1, lat, lon);
        times   = RST(file, MOON, jdate, lat, lon);
        times_f = RST(file, MOON, jdate + 1, lat, lon);
        result->moon.tru = AdjustTimes(times_p.trn, times.trn, times_f.trn, gmt_offset);

        fclose(file);

        // if western hemisphere or Britian Summer Time
        if (gmt_offset > 0)
        {
            // if the following Greenwich day has a primary phase at a time that is during the
            // current local day, use the phase from the following Greenwich day
            if  (   (jpl[f_day].phase_time != 0)
                    &&  (jpl[f_day].phase_time <= abs(gmt_offset)) )
            {
                result->moon_phase = jpl[f_day].phase;
                phase_time = jpl[f_day].phase_time;
            }
            // if the current Greenwich day has a primary phase at a time that is not during the
            // current local day, use the phase from the following Greenwich day 
            else if (   (jpl[c_day].phase_time != 0)
                    &&  (jpl[c_day].phase_time <= abs(gmt_offset)) )
            {
                result->moon_phase = jpl[f_day].phase;
                phase_time = jpl[f_day].phase_time;
            }
            // if it is not necessary to use the phase from the following GMT day, use the phase
            // from the current GMT day
            else
            {
                result->moon_phase = jpl[c_day].phase;
                phase_time = jpl[c_day].phase_time;
            }
            if (phase_time != 0.0)
            {
                phase_time += 24 * 60;
                phase_time -= abs(gmt_offset);
                if (phase_time > MINUTES_PER_DAY)
                {
                    phase_time -= 24 * 60;
                }
            }
            result->phase_time = phase_time;
        }
        // if eastern hemisphere
        else if (gmt_offset < 0)
        {
            // if the previous Greenwich day has a primary phase at a time that is during the
            // current local day, use the phase from the following Greenwich day
            if  (   (jpl[p_day].phase_time != 0)
                &&  ((MINUTES_PER_DAY - jpl[p_day].phase_time) <= abs(gmt_offset)))
            {
                result->moon_phase = jpl[p_day].phase;
                phase_time = jpl[p_day].phase_time;
            }
            // if the current Greenwich day has a primary phase at a time that is not during the
            // current local day, use the hase from the previous Greenwich day 
            else if (   (jpl[c_day].phase_time != 0)
                    &&  ((MINUTES_PER_DAY - jpl[c_day].phase_time) <= abs(gmt_offset)))
            {
                result->moon_phase = jpl[p_day].phase;
                phase_time = jpl[p_day].phase_time;
            }
            // if it is not necessary to use the phase from the previous GMT day, use the phase
            // from the current GMT day
            else
            {
                result->moon_phase = jpl[c_day].phase;
                phase_time = jpl[c_day].phase_time;
            }
            if (phase_time != 0.0)
            {
                phase_time += abs(gmt_offset);
                {
                    if (phase_time > MINUTES_PER_DAY)
                    {
                        phase_time -= MINUTES_PER_DAY;
                    }
                }
            }
            result->phase_time = phase_time;
        }
        // if the gmt offset is 0, use the phase from the current Greenwich day
        else
        {
            result->moon_phase = jpl[c_day].phase;
            phase_time = jpl[c_day].phase_time;
            result->phase_time = phase_time;
        }
        result->moon_illum = jpl[c_day].illum;
        success = EXIT_SUCCESS;
    }
    else
    {
        //rb_eval_string("puts 'No file found at: '+`pwd`");
        success = DATA_FAIL;
    }
    return success;
}

/***********************   Get Moon Illumination Percentage Data for One Year

This function processes the moon's percent illumination data from the specified file and places
it into jpl_temp[] for one year. Parameter index is kept up to date as this function is called 
multiple times - one for each year.

RETURN VALUE:   EXIT_SUCCESS or Error Code

PARAMETERS:     current file
                index into jpl_temp[]
*/
int GetIllumYear(const char *filename, int *index)
{
    FILE* file;
    char buffer[31][102];
    int col[] = { 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96 };
    int m;
    int d;
    int n;
    int success;

    file = fopen(filename, "r");
    if (file != NULL)
    {
        for (n = 0; n < 31; n++)
        {
            fgets(buffer[n], 102, file);
        }
        fclose(file);
        for (m = 0; m < 12; m++)
        {
            for (d = 0; d < 31; d++)
            {
                if (buffer[d][col[m]] == '*')
                {
                    break;
                }
                else
                {
                    jpl_temp[*index].illum = atoi(buffer[d] + col[m]);
                    (*index) ++;
                }
            }
        }
        success = EXIT_SUCCESS;
    }
    else
    {
        success = EXIT_FAILURE;
    }
    return success;
}

/***********************   Format the Time Structure

This function formats a decimal time (H.hhhh) into minutes and seconds. Internal to the 
program, time of day is kept in interger munites. To avoid floating point, seconds are a
separate field and is used only for development and desting.

RETURN VALUE:   formated time structure

PARAMETERS:     decimal time
*/
time_type FmtTime(double d_time)
{
    time_type result;
    double d_minute;
    double d_second;
    double i_minute;
    double i_second;
    double temp;

    d_minute = d_time * 60;
    i_minute = floor(d_minute);
    d_second = (d_minute - i_minute) * 60;
    i_second = floor(d_second);
    temp = (d_second - i_second);
    if (temp >= 0.5)
    {
        i_second += 1.0;
    }
    if (i_second == 60)
    {
        i_second = 0;
        i_minute += 1;
    }
    result.minute = (int)d_minute;
    result.second = (int)d_second;
    result.exception = NULL;

    return result;
}

/***********************   Format a Time String

This function formats a string in the format hh:mm:ss from a time_type structure.

RETURN VALUE:   none

PARAMETERS:     pointer to destination string
                time-type structure to be formatted
                flag indicating whether format should 24 hour (with seconds) or am/pm
*/
void FmtTimeStr(char *string, time_type times, int am_pm)
{
    int n;
    int hour;
    int min;
    char a_p;
    const char exception[9][9] = {  
        "        ",
        "        ",
        "  NONE  ",
        "        ",
        "  DARK  ",
        " BRIGHT ",
        "        ",
        "ERROR  1",
        "ERROR  2" };

    if (times.exception >= NONE_TODAY)
    {
        sprintf(string, "%s", exception[times.exception - 1]);
    }
    else
    {
        if (am_pm == YES)
        {
            if (times.second >= 30) times.minute += 1;
            if (times.minute >= MINUTES_PER_DAY) times.minute = MINUTES_PER_DAY - 1;
            hour = times.minute / 60;
            a_p = hour >= 12? 'p': 'a';
            if (hour > 12) hour -= 12;
            if (hour == 0) hour = 12;
            min = times.minute % 60;
            sprintf(string, "%2d:%2d %cm", hour, min, a_p);
            if (string[3] == ' ') string[3] = '0';
        }
        else
        {
            hour = times.minute / 60;
            min = times.minute % 60;
            sprintf(string, "%2d:%2d:%2d", hour, min, times.second);
            for (n = 0; n < 8; n++)
            {
                if (string[n] == ' ') string[n] = '0';
            }
        }
    }
}

/***********************   Adjust Times

For an explanation of this function,see the Adjust Times files in the project folder.

RETURN VALUE:   adjusted time structure

PARAMETERS:     time sStructure for the previous day
                time structure for the Current Day
                time structure for the Following Day
                offset from GMT (hours west)
*/
time_type AdjustTimes(  time_type previous_day, time_type current_day,
                        time_type following_day, int gmt_offset)
{
    #define TIME_BEFORE_MIDNIGHT(x) (MINUTES_PER_DAY - x)
    #define TIME_AFTER_MIDNIGHT(x)  x
    time_type result;
    int special;
    int temp_1;
    int temp_2;

    // if in the western hemisphere or Britan Summer Time
    if (gmt_offset > 0)
    {
        special = abs(current_day.minute - following_day.minute) > (MINUTES_PER_DAY / 2) ? YES : NO;
        if ((current_day.exception > ROUTINE) || (following_day.exception > ROUTINE))
        {
            if (current_day.exception > ERROR) result = following_day;
            else result = current_day;
            result.minute += MINUTES_PER_DAY;
            result.minute -= gmt_offset;
            result.minute %= MINUTES_PER_DAY;
        }
        else if (TIME_AFTER_MIDNIGHT(current_day.minute) >= gmt_offset)
        {
            result = current_day;
            result.minute -= gmt_offset;
        }
        else if (TIME_AFTER_MIDNIGHT(following_day.minute) < gmt_offset)
        {
            result = following_day;
            result.minute += MINUTES_PER_DAY;
            result.minute -= gmt_offset;
        }
        else if (special == YES)
        {
            result = current_day;
            temp_1 = (current_day.minute + MINUTES_PER_DAY - gmt_offset) % MINUTES_PER_DAY;
            temp_2 = (following_day.minute + MINUTES_PER_DAY - gmt_offset) % MINUTES_PER_DAY;
            result.minute = (temp_1 + temp_2) / 2;
        }
        else
        {
            result.exception = NONE_TODAY;
        }
    }
    //if in eastern hemisphere
    else if (gmt_offset < 0)
    {
        gmt_offset = abs(gmt_offset);
        special = abs(previous_day.minute - current_day.minute) > (MINUTES_PER_DAY / 2)? YES: NO;
        if ((current_day.exception >    ROUTINE) || (previous_day.exception > ROUTINE))
        {
            if (previous_day.exception > ERROR) result = current_day;
            else result = previous_day;
            result.minute += gmt_offset;
            result.minute %= MINUTES_PER_DAY;
        }
        else if (TIME_BEFORE_MIDNIGHT(previous_day.minute) <= gmt_offset)
        {
            result = previous_day;
            result.minute += gmt_offset;
            result.minute %= MINUTES_PER_DAY;
        }
        else if (   (TIME_BEFORE_MIDNIGHT(previous_day.minute) > gmt_offset)
                &&  (TIME_BEFORE_MIDNIGHT(current_day.minute) > gmt_offset) )
        {
            result = current_day;
            result.minute += gmt_offset;
        }
        else if (special == YES)
        {
            result = current_day;
            temp_1 = (previous_day.minute + gmt_offset) % MINUTES_PER_DAY;
            temp_2 = (current_day.minute + gmt_offset) % MINUTES_PER_DAY;
            result.minute = (temp_1 + temp_2) / 2;
        }
        else
        {
            result.exception = NONE_TODAY;
        }
    }
    else
    {
        result = current_day;
    }
    return result;
}

/***********************   Rise Set Transit Transit Underfoot

This function is the heart of the original Heafner code. It has been substantially changed as
follows:

1) The original function got latitude and longitude data from file scope variables obser_lat and
obser_lon. Those have been replaced by parameters lat and lon.

2) The original function returned results as text strings. These have been replaced by a singgle
rst_type structure which is comprised of time type structures. Each time type strucuture
includes exception codes.

3) The original function reported exceptions (object never rises or sets, event occurs previous
or following day) as text strings. These have been replaced by the exceptions member of the
structure.

4) The rsflag has been repurposed to show the exception.

5) The original function got the ra[] and dec[] arrays as parameters. These are now determined
from the JPL file.

6) The original function had delta t as a parameter. This is now a fixed value of 69 which is
average between the 2016 value of 68.5 and the 2020 value of 69.5.

7) Added a fudge factor to z0 to make moon rise and set times agree with published data.

RETURN VALUE:   RST structure

PARAMETERS:     handle to the cata file - already opened.
                object - SUN or MOON
                julian date
                latitude
                longitude
*/
rst_type RST(FILE *file, int object, double jed, double lat, double lon)
{
    int rsflag, c;
    double h0, cosh0, newm, oldm, m, m0, m1, m2;
    double ristime, settime, trntime, gast0;
    double d1, d2, d3, r1, r2, r3;
    double deltat;
    double ra[3];
    double dec[3];
    double z0;
    double hp;
    double sd;
    int index;
    int rst_tries;
    rst_type result;
    jpl_type jpl[3];

    deltat = 69;
    index = (int)(floor(jed) - floor(JDATE_BASE));
    fseek(file, ((index - 1) * sizeof(jpl_type)), SEEK_SET);
    fread(&jpl[0], sizeof(jpl_type), 3, file);
    if (object == SUN)
    {
        ra[0] = D2R * jpl[0].sun.ra;
        dec[0] = D2R * jpl[0].sun.dec;
        ra[1] = D2R * jpl[1].sun.ra;
        dec[1] = D2R * jpl[1].sun.dec;
        ra[2] = D2R * jpl[2].sun.ra;
        dec[2] = D2R * jpl[2].sun.dec;
        z0 = D2R * deg(90.34 + 0.16);
    }
    else
    {
        ra[0] = D2R * jpl[0].moon.ra;
        dec[0] = D2R * jpl[0].moon.dec;
        ra[1] = D2R * jpl[1].moon.ra;
        dec[1] = D2R * jpl[1].moon.dec;
        ra[2] = D2R * jpl[2].moon.ra;
        dec[2] = D2R * jpl[2].moon.dec;
        hp = D2R * deg(0.444156);
        sd = asin(0.272493 * sin(hp));
        z0 = D2R * deg(90.34) + sd - hp;
        z0 -= 0.00275;          // fudge factor
    }
    /* Make sure the ra[]'s are in continuous order */

    if ((ra[1] < ra[0]) && (ra[2] > ra[1])) {
        ra[1] = ra[1] + TWOPI;
        ra[2] = ra[2] + TWOPI;
    }
    else if ((ra[1] > ra[0]) && (ra[2] < ra[1])) {
        ra[2] = ra[2] + TWOPI;
    }

    r1 = ra[1] - ra[0];
    r2 = ra[2] - ra[1];
    r3 = r2 - r1;
    d1 = dec[1] - dec[0];
    d2 = dec[2] - dec[1];
    d3 = d2 - d1;

    rsflag = 0;
    cosh0 = (cos(z0) - sin(lat) * sin(dec[1])) / (cos(lat) * cos(dec[1]));
    if (cosh0 < -1.0)
    {
        // Object - never sets
        rsflag = NEVER_SET;
    }
    else if (cosh0 > 1.0)
    {
        // Object never rises 
        rsflag = NEVER_RISE;
    }

    GetGST(jed, 1, &gast0);

    m0 = (ra[1] - lon - gast0) / TWOPI;
    m0 = amodulo(m0, 1.0);

    if (rsflag == 0)
    {
        h0 = acos(cosh0);
        h0 = amodulo(h0, PI);
        m1 = m0 - h0 / TWOPI;
        m1 = amodulo(m1, 1.0);
        m2 = m0 + h0 / TWOPI;
        m2 = amodulo(m2, 1.0);

        // Rising 
        oldm = m1;
        c = 1;
        rst_tries = RST_Interpolate(c, z0, oldm, gast0, deltat, ra, dec,
                                    r1, r2, r3, d1, d2, d3, lat, lon, &newm);
        if (rst_tries == RST_MAX_TRIES)
        {
            result.ris.exception = RST_ERROR;
        }
        else if (rst_tries > RST_MAX_TRIES)
        {
            result.ris.exception = RST_FAIL;
        }
        else
        {
            m = newm;
            ristime = 24.0 * m;

            if (ristime > 24.0) {
                ristime = ristime - 24.0;
                // Event occurs the following day 
                result.ris = FmtTime(ristime);
                result.ris.exception = NEXT_DAY;
            }
            else if (ristime < 0.0) {
                ristime = ristime + 24.0;
                // Event occurs the previous day 
                result.ris = FmtTime(ristime);
                result.ris.exception = PREV_DAY;
            }
            else {
                result.ris = FmtTime(ristime);
            }
        }
        // Setting 
        oldm = m2;
        c = 1;
        rst_tries = RST_Interpolate(c, z0, oldm, gast0, deltat, ra, dec,
                                    r1, r2, r3, d1, d2, d3, lat, lon, &newm);
        if (rst_tries == RST_MAX_TRIES)
        {
            result.set.exception = RST_ERROR;
        }
        else if (rst_tries > RST_MAX_TRIES)
        {
            result.set.exception = RST_FAIL;
        }
        else
        {
            m = newm;
            settime = 24.0 * m;
            if (settime > 24.0)
            {
                settime = settime - 24.0;
                result.set = FmtTime(settime);
                result.set.exception = NEXT_DAY;
            }
            else if (settime < 0.0) {
                settime = settime + 24.0;
                result.set = FmtTime(settime);
                result.set.exception = PREV_DAY;
            }
            else {
                result.set = FmtTime(settime);
            }
        }
    }

    else
    {
        result.set = FmtTime(0);
        result.ris = FmtTime(0);
        result.set.exception = rsflag;
        result.ris.exception = rsflag;
    }
    // Transiting 
    oldm = m0;
    c = 0;
    rst_tries = RST_Interpolate(c, z0, oldm, gast0, deltat, ra, dec,
                                r1, r2, r3, d1, d2, d3, lat, lon, &newm);
    if (rst_tries == RST_MAX_TRIES)
    {
        result.trn.exception = RST_ERROR;
    }
    if (rst_tries > RST_MAX_TRIES)
    {
        result.trn.exception = RST_FAIL;
    }
    else
    {
        m = newm;
        trntime = 24.0 * m;
        if (trntime > 24.0) {
            trntime = trntime - 24.0;
            result.trn = FmtTime(trntime);
            result.trn.exception = NEXT_DAY;
        }
        else if (trntime < 0.0) {
            trntime = trntime + 24.0;
            result.trn = FmtTime(trntime);
            result.trn.exception = PREV_DAY;
        }
        else {
            result.trn = FmtTime(trntime);
        }
    }
    return result;
}

/***********************   Interpolation required by RST()

This  is original Heafner code. For explanation, see the originalfunction in astrolib.c.
The only modification is that the original function got latitude and longitude data from
file scope variables obser_lat and obser_lon. Those have been replaced by parameters lat
and lon.

RETURN VALUE:   count of inerations in do loop

PARAMETERS:     lat     latitude
                lon     longitude
                (see original function in astrolib to describe others)
*/
static int RST_Interpolate(int c, double z0, double oldm, double gast0,
    double deltat, double *ra, double *dec, double r1, double r2, double r3,
    double d1, double d2, double d3, double lat, double lon, double *newm) {

    double alpha, dm, h, gast, delta, alt, n;
    int count;

    count = 0;
    *newm = oldm;
    do {
        count ++;
        if (count > RST_MAX_TRIES)
        {
            break;
        }
        gast = gast0 + 6.300388093 * (*newm);
        gast = amodulo(gast, TWOPI);
        n = *newm + deltat / 86400.0;
        alpha = ra[1] + 0.5 * n * (r1 + r2 + n * r3);
        alpha = amodulo(alpha, TWOPI);
        delta = dec[1] + 0.5 * n * (d1 + d2 + n * d3);
        h = gast + lon - alpha;
        alt = asin(sin(delta) * sin(lat) + cos(delta) * cos(lat) * cos(h));
        if (c == 0) {
            /* h must satisfy -PI <= h <= PI */
            h = amodulo(h, TWOPI);
            if (h > PI) {
                h = h - TWOPI;
            }
            dm = -h / TWOPI;
        }
        else {
            dm = (alt - PIDIV2 + z0) / (TWOPI * cos(delta) * cos(lat) * sin(h));
        }
        *newm = (*newm) + dm;


    } while (fabs(dm) >= 1e-15);
    if ((*newm >= 2.0) || (*newm <= -1.0))
    {
        count = RST_MAX_TRIES;
    }
    return count;
}

/***********************   Convert Degrees

This function cnverts degrees in the format dd.mmss to decimal degrees.
It is original Heafner code with no changes

RETURN VALUE:   decimal degrees

PARAMETERS:     degrees in format dD.MMSS
*/
double deg(double x) {

    double dd, d, fixdd, ddfixdd;

    if (x == 0.0) return (0.0);

    dd = fabs(x);
    fixdd = floor(dd);
    ddfixdd = dd - fixdd + 5.0e-10;  /* fudge factor */
    d = fixdd + floor(100.0 * ddfixdd) / 60.0;
    d = d + (10000.0 * ddfixdd - 100.0 * floor(100.0 * ddfixdd)) / 3600.0;

    return ((x / dd) * d);
}

/***********************   Get Grenwich Sidereal Time

This function computes Sidereal time (in radians) from the Julian date.It is original Heafner
code excdpt:

1) The original was siginicantly more complicated in that it computed apparent sidereal time.
We don't require that kind of (sub-minute) accuracy, so we will use mean sidereal time. All
code required to calculate apparent sidereal time has been removed.

2) The parameter that specified mean or apparent sideereal time has been removed.

3) The original had logic such that saved the time and apparent flag. If the call was the
same as the previous call, the function would simply return with no changes. That would force
the calling function to save the result for use if it happened to call this function twice in
a row with the same date. That may have been useful when being called by some functions not
used in our application, but where it is called in RST, the result (gast0) is not saved. The
result was that an unitialized value was sent to RST_Interpolate() which caused it to fail.
That logic has been removed.

RETURN VALUE:   none

PARAMETERS:     Juilian date
                address of result
*/
void GetGST(double jed, int s, double *gst) {

    double T;

    T = (jed - J2000) / JulCty;

    /* compute GMST in seconds */
    *gst = 67310.54841 + T * ((876600.0 * 3600.0 + 8640184.812866)
        + T * (0.093104 + T * (-0.0000062)));

    /* convert to radians */
    *gst = amodulo(*gst / 3600.0, 24.0) * H2R;
    *gst = amodulo(*gst, TWOPI);
}

/***********************   Calculate the modulo of an angle

This function calculates the module of an angle to the specified base. It us used with values in
radians with either PI or TWOPI

RETURN VALUE:   modulo of the angle

PARAMETERS:     the angle
                the base
*/
double amodulo(double a, double b) {

    double x;

    x = a - b * floor(a / b);
    return (x);
}

/***************************  Ruby interface code *******************************************************/

static VALUE
test_function(VALUE self, VALUE number) {
    VALUE v;
    long i;
    i = FIX2LONG(number);
    i = i*2;
    v = LONG2FIX(i);
    return v;
}

/* This function initializes the new Solunar object - the names should match, and capitalization seems to come from Ruby magic.
*  A "Value" is C/Ruby.h/C++ speak for a Ruby object. In this case, with the rb_intern stuff, that object is of the class "Solunar" and is registered as such.
*  Registration is pure ruby magic. rb_define_method uses this, of the format (root_object - always the thing declared before, name to be registered as, function name, number of arguements to pass)
*  Caveat to the above: Functions that will return to Ruby always are of type "Value", and always have a first object, of type value, that does something likely important - but exactly what, I couldn't tell you...
*/

void
Init_solunar(void) {
    VALUE cSolunar;

    cSolunar = rb_const_get(rb_cObject, rb_intern("Solunar"));

    rb_define_method(cSolunar, "multi", test_function, 1);
    rb_define_method(cSolunar, "generate", generate, 7);
}

