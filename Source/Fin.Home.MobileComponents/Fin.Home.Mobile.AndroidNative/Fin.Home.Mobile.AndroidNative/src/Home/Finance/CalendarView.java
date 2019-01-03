package Home.Finance;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;
import java.util.List;
import java.util.Locale;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.Typeface;
import android.os.Bundle;
import android.text.format.DateFormat;
import android.view.*;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.widget.*;



public class CalendarView extends Activity implements OnClickListener
{

	private LinearLayout _layoutScreen;
    private TextView _txtCredits;
    private TextView _txtDebits;
    private Button currentMonth;
    private ImageView prevMonth;
    private ImageView nextMonth;
    private GridView calendarView;
    private GridCellAdapter adapter;
    private Calendar _calendar;
    private int month, year;
    private SwipeListner _globalSwipeDetector;
    private final DateFormat dateFormatter = new DateFormat();
    private static final String dateTemplate = "MMMM yyyy";

    @Override
    protected void onSaveInstanceState(Bundle icicle)
    {
        super.onSaveInstanceState(icicle);
        icicle.putString("SelectedCalendar", (String)DateUtility.GetFileDateString(_calendar));
    }
    /**
     * Called when the activity is first created.
     */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);        
        
        if(!FinanceCommon.IsDatabaseAvailable())
        {
        	new AlertDialog.Builder(this).setTitle("File not found").setMessage("Unable to open finance database.").show();
        	finish();
        	return;
        }
        
        if(savedInstanceState != null)
        {
            String  selCal = savedInstanceState.getString("SelectedCalendar");
            
            if(selCal != null && !selCal.isEmpty())
            {
                _calendar = DateUtility.GetDate(selCal, "MM/dd/yyyy");
            }
        }
        
        if(_calendar == null)
        {
            _calendar = Calendar.getInstance(Locale.getDefault());
        }        
        
        month = _calendar.get(Calendar.MONTH) + 1;
        year = _calendar.get(Calendar.YEAR);
        
        setContentView(R.layout.calendar_view);
        
        FinanceCommon.AppContext = getApplicationContext();

        _layoutScreen = (LinearLayout)findViewById(R.id.calendar_screen);
        //selectedDayMonthYearButton = (Button) this.findViewById(R.id.selectedDayMonthYear);
        //selectedDayMonthYearButton.setText("Selected: ");

        prevMonth = (ImageView) this.findViewById(R.id.prevMonth);
        prevMonth.setOnClickListener(this);

        currentMonth = (Button) this.findViewById(R.id.currentMonth);        

        nextMonth = (ImageView) this.findViewById(R.id.nextMonth);
        nextMonth.setOnClickListener(this);

        calendarView = (GridView) this.findViewById(R.id.calendar);
        _txtCredits = (TextView)findViewById(R.id.txtCredits);
        _txtDebits = (TextView)findViewById(R.id.txtDebits);        
        
        
        _globalSwipeDetector = new SwipeListner(getApplicationContext());
        
        _layoutScreen.setOnTouchListener(_globalSwipeDetector);
        calendarView.setOnTouchListener(_globalSwipeDetector);        
    }

    @Override
	public void onResume()
	{
		super.onResume();
		
		FinanceAdapter.GetSingleton().RefreshExpenseLookupDataset();
		
		ReDrawCurrent();
	}

    
    /**
     *
     * @param month
     * @param year
     */
    private void setGridCellAdapterToDate(int month, int year)
    {
        adapter = new GridCellAdapter(getApplicationContext(), R.id.calendar_cell, month, year);
        _calendar.set(year, month - 1, _calendar.get(Calendar.DAY_OF_MONTH));
        currentMonth.setText(dateFormatter.format(dateTemplate, _calendar.getTime()));
        adapter.notifyDataSetChanged();
        calendarView.setAdapter(adapter);
    }

    private void SetMonthYear(int mon, int year)
    {
        this.year = year;
        month = mon;
        ReDrawCurrent();
    }
    
    private void ReDrawCurrent()
    {
    	setGridCellAdapterToDate(month, year);
    }

    @Override
    public void onClick(View v)
    {
        if (v == prevMonth)
        {
            if (month <= 1)
            {
                month = 12;
                year--;
            }
            else
            {
                month--;
            }

            ReDrawCurrent();
        }
        if (v == nextMonth)
        {
            if (month > 11)
            {
                month = 1;
                year++;
            }
            else
            {
                month++;
            }

            ReDrawCurrent();
        }

    }

    @Override
    public void onDestroy()
    {

        super.onDestroy();
    }
    
    private int EXP_ALL = Menu.FIRST + 1;
    private int DEL_ALL = Menu.FIRST + 2;
    
    @Override
    public boolean onCreateOptionsMenu(Menu menu)
    {
        menu.add(0, EXP_ALL, 0, "Export All");
        menu.add(0, DEL_ALL, 0, "Delete All");

        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
            int selected = item.getItemId();
            if (selected == EXP_ALL)
            {
                CharSequence status = FinanceAdapter.GetSingleton().ExportAll();
                if(status == "") status = "Export-Successful";
                new AlertDialog.Builder(this).setTitle("Export Status").setMessage(status).show();
            }
            if (selected == DEL_ALL)
            {              
                new AlertDialog.Builder(this).setTitle("Confirm Delete").setMessage("Delete all expense records?")
                .setPositiveButton("Yes",
                new android.content.DialogInterface.OnClickListener()
                {
                    public void onClick(DialogInterface arg0, int arg1)
                    {                        
                        boolean status = FinanceAdapter.GetSingleton().RemoveAll();
                        Toast.makeText(getApplicationContext(), status ? "Cleared" : "Failed to clear items", Toast.LENGTH_SHORT).show();
                    }                
                }
                )
                .setNegativeButton("No", null)
                .show();
                
                ReDrawCurrent();
            }
            return super.onOptionsItemSelected(item);
    }

    
    public class SwipeListner extends SwipeTouchListener
    {
    	public SwipeListner(Context cntxt)
    	{    	
    		super(cntxt);
    	}
    	
    	public void onSwipeTop() {
        	onClick(nextMonth);
        }
        public void onSwipeRight() {
        	onClick(prevMonth);
        }
        public void onSwipeLeft() {
        	onClick(nextMonth);
        }
        public void onSwipeBottom() {
        	onClick(prevMonth);
        }
    };
// ///////////////////////////////////////////////////////////////////////////////////////
// Inner Class
    public class GridCellAdapter extends BaseAdapter implements OnClickListener
    {
        
        private final Context _context;
        private final List<String> list;
        private static final int DAY_OFFSET = 1;
        private final String[] weekdays = new String[]
        {
            "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
        };
        private final String[] months =
        {
            "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
        };
        private final int[] daysOfMonth =
        {
            31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
        };
        private final int month, year;
        private int daysInMonth;
        private int currentDayOfMonth;
        private Button gridcell;
        private TextView _txtDailyCredits;
        private TextView _txtDailyDebits;

        // Days in Current Month
        public GridCellAdapter(Context context, int textViewResourceId, int month, int year)
        {
            super();
            this._context = context;
            this.list = new ArrayList<String>();
            this.month = month;
            this.year = year;


            Calendar calendar = Calendar.getInstance();
            setCurrentDayOfMonth(calendar.get(Calendar.DAY_OF_MONTH));
            // Print Month
            printMonth(month, year);
        }

        private String getMonthAsString(int i)
        {
            return months[i];
        }

        private String getWeekDayAsString(int i)
        {
            return weekdays[i];
        }

        private int getNumberOfDaysOfMonth(int i)
        {
            return daysOfMonth[i];
        }

        public String getItem(int position)
        {
            return list.get(position);
        }

        @Override
        public int getCount()
        {
            return list.size() + 7;
        }

        /**
         * Prints Month
         *
         * @param mm
         * @param yy
         */
        private void printMonth(int mm, int yy)
        {
        	int currentMonth = mm - 1;
        	FinanceAdapter.GetSingleton().Prefetch(currentMonth, yy);
        	
            // The number of days to leave blank at
            // the start of this month.
            int trailingSpaces = 0;
            int leadSpaces = 0;
            int daysInPrevMonth = 0;
            int prevMonth = 0;
            int prevYear = 0;
            int nextMonth = 0;
            int nextYear = 0;

            
            String currentMonthName = getMonthAsString(currentMonth);
            daysInMonth = getNumberOfDaysOfMonth(currentMonth);

            Calendar curCal = Calendar.getInstance();

            // Gregorian Calendar : MINUS 1, set to FIRST OF MONTH
            GregorianCalendar cal = new GregorianCalendar(yy, currentMonth, 1);
            


            if (currentMonth == 11)
            {
                prevMonth = currentMonth - 1;
                daysInPrevMonth = getNumberOfDaysOfMonth(prevMonth);
                nextMonth = 0;
                prevYear = yy;
                nextYear = yy + 1;

            } else if (currentMonth == 0)
            {
                prevMonth = 11;
                prevYear = yy - 1;
                nextYear = yy;
                daysInPrevMonth = getNumberOfDaysOfMonth(prevMonth);
                nextMonth = 1;

            } else
            {
                prevMonth = currentMonth - 1;
                nextMonth = currentMonth + 1;
                nextYear = yy;
                prevYear = yy;
                daysInPrevMonth = getNumberOfDaysOfMonth(prevMonth);

            }

            // Compute how much to leave before before the first day of the
            // month.
            // getDay() returns 0 for Sunday.
            int currentWeekDay = cal.get(Calendar.DAY_OF_WEEK) - 1;
            trailingSpaces = currentWeekDay;



            if (cal.isLeapYear(cal.get(Calendar.YEAR)) && mm == 1)
            {
                ++daysInMonth;
            }
            
            // Trailing Month days
            for (int i = 0; i < trailingSpaces; i++)
            {

                list.add(String.valueOf((daysInPrevMonth - trailingSpaces + DAY_OFFSET) + i) + "-GREY" + "-" + (prevMonth) + "-" + prevYear);
            }

            // Current Month Days
            for (int i = 1; i <= daysInMonth; i++)
            {

                if (i == getCurrentDayOfMonth() && currentMonth == curCal.get(Calendar.MONTH) && yy == curCal.get(Calendar.YEAR))
                {
                    list.add(String.valueOf(i) + "-BLUE" + "-" + (currentMonth) + "-" + yy);
                } else
                {
                    list.add(String.valueOf(i) + "-WHITE" + "-" + (currentMonth) + "-" + yy);
                }
            }

            // Leading Month days
            for (int i = 0; i < list.size() % 7; i++)
            {

                list.add(String.valueOf(i + 1) + "-GREY" + "-" + (nextMonth) + "-" + nextYear);
            }
            
            double totalCredits = FinanceAdapter.GetSingleton().GetAmoutPerMonth(currentMonth, yy, false);
            double totalDebits = FinanceAdapter.GetSingleton().GetAmoutPerMonth(currentMonth, yy, true);
            
            if(totalCredits > 0)
            {
                _txtCredits.setText("Credits: " + FinanceCommon.GetDoubleAsString(totalCredits) + "/-");
            }
            else                
            {
                _txtCredits.setText("");
            }
             
            if(totalDebits > 0)
            {
                _txtDebits.setText("Debits: " + FinanceCommon.GetDoubleAsString(totalDebits) + "/-");
            }
            else                
            {
                _txtDebits.setText("");
            }
        }

        @Override
        public long getItemId(int position)
        {
            return position;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent)
        {
            boolean isHoliday = position % 7 == 0 || position % 7 == 6;
            
            View row = convertView;
            if (row == null)
            {
                LayoutInflater inflater = (LayoutInflater) _context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                row = inflater.inflate(R.layout.calendar_cell, parent, false);
                
                row.setOnTouchListener(_globalSwipeDetector);
            }

            // Get a reference to the Day gridcell
            gridcell = (Button) row.findViewById(R.id.calendar_cell);
            
//            if(convertView == null)
//            {
//                gridcell.setOnTouchListener(_globalSwipeDetector);
//            }
            
            if(isHoliday)
            {
                gridcell.setTextColor(Color.RED);
            }
            else
            {
                gridcell.setTextColor(Color.WHITE);
            }
            
            int lstIndex = position;
            
            if(position > 6)
            {
                gridcell.setOnClickListener(this);
                lstIndex = position - 7;
            }
            else
            {
                //gridcell.setTypeface(null, Typeface.BOLD);
                gridcell.setText(weekdays[position].substring(0, 3));   
                return row;
            }

            // ACCOUNT FOR SPACING


            String[] day_color = list.get(lstIndex).split("-");
            String theday = day_color[0];
            String themonth = day_color[2];
            String theyear = day_color[3];
            
         // Set the Day GridCell
            gridcell.setText(theday);
            gridcell.setTag(theday + "-" + themonth + "-" + theyear);
            
            Calendar date = DateUtility.GetDate(GetSelectedDate(gridcell.getTag().toString()), "dd-MM-yyyy");
              
            double itemCount = FinanceAdapter.GetSingleton().GetAmoutPerDate(date, true);
            
            if (itemCount > 0)
            {
            	 _txtDailyDebits = (TextView) row.findViewById(R.id.num_events_per_day);
            	 _txtDailyDebits.setText(FinanceCommon.GetDoubleAsString(itemCount));
            }
            
           itemCount = FinanceAdapter.GetSingleton().GetAmoutPerDate(date, false);
            
            if (itemCount > 0)
            {
            	 _txtDailyCredits = (TextView) row.findViewById(R.id.credits_per_day);
            	 _txtDailyCredits.setText(FinanceCommon.GetDoubleAsString(itemCount));
            }

            if (day_color[1].equals("GREY"))
            {
                gridcell.setTextColor(Color.LTGRAY);
            }
//            if (day_color[1].equals("WHITE"))
//            {
//                gridcell.setTextColor(Color.WHITE);
//            }
            if (day_color[1].equals("BLUE"))
            {
                gridcell.setTextColor(getResources().getColor(R.color.static_text_color));
            }
            return row;
        }
        
        private String GetSelectedDate(String dateStr)
        {
        	 String[] tokens = dateStr.split("-");

             int day = Integer.parseInt(tokens[0]);
             int mon = Integer.parseInt(tokens[1]);
             int year = Integer.parseInt(tokens[2]);
             
             return String.valueOf(day) + "-" + String.valueOf(mon+1) + "-" + String.valueOf(year);
        }
        
        @Override
        public void onClick(View view)
        {   
            //Button btn = (Button) view;
            //btn.setTextColor(Color.GREEN); 

            String date_month_year = (String) view.getTag();
            //selectedDayMonthYearButton.setText("Selected: " + date_month_year);

            try
            {               
            	DateUtility.SelectedDate = DateUtility.GetDate(GetSelectedDate(date_month_year), "dd-MM-yyyy");
            	
                Intent intent = new Intent(getApplicationContext(), Home.Finance.DailyActivity.class);                
                //intent.putExtra("Date", DateUtility.SelectedDateAsString());
                startActivity(intent);
            
//                if (btn.getCurrentTextColor() == Color.LTGRAY)
//                {
//                    SetMonthYear(mon + 1, year);
//                }

                

            } catch (Exception e)
            {
                e.printStackTrace();
            }
        }

        public int getCurrentDayOfMonth()
        {
            return currentDayOfMonth;
        }

        private void setCurrentDayOfMonth(int currentDayOfMonth)
        {
            this.currentDayOfMonth = currentDayOfMonth;
        }
    }
}
