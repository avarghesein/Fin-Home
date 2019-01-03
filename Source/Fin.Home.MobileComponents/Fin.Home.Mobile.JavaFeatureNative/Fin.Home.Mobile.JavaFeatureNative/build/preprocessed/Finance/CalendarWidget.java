package Finance;

import java.util.Calendar;
import java.util.Date;
import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Font;
import javax.microedition.lcdui.Graphics;

public class CalendarWidget
{
	static final String[] MONTH_LABELS = new String[]{
		"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};
	static final String[] WEEKDAY_LABELS = new String[]{
		"M", "T", "W", "T", "F", "S", "S"
	};
        
        public static String GetDateString(Date dte)
        {
            Calendar cal = Calendar.getInstance();
            cal.setTime(dte);
            
            String dteStr = cal.get(Calendar.DAY_OF_MONTH) + "-" + MONTH_LABELS[cal.get(Calendar.MONTH)] + "-" + cal.get(Calendar.YEAR);
            return dteStr;
        }
        
        public static String GetRecordStoreDateString(Date dte)
        {
            Calendar cal = Calendar.getInstance();
            cal.setTime(dte);
            
            String dteStr = ((int)cal.get(Calendar.MONTH) + (int)1) + "/" + cal.get(Calendar.DAY_OF_MONTH) + "/" + cal.get(Calendar.YEAR);
            return dteStr;
        }
        
        public static long GetRecordTimestamp()
        {
            Calendar cal = Calendar.getInstance();
            cal.set(Calendar.DAY_OF_MONTH, 01);
            cal.set(Calendar.MONTH, Calendar.JANUARY);
            cal.set(Calendar.YEAR, 2000);
            cal.set(Calendar.HOUR, 00);
            cal.set(Calendar.MINUTE, 00);
            cal.set(Calendar.SECOND, 00);
            cal.set(Calendar.AM_PM, Calendar.AM);            
            Date dte2000 = cal.getTime();
            Date dtNow = new Date();            
            return ( (dtNow.getTime() - dte2000.getTime()) / 1000);
        }
        
	/* starting week day: 0 for monday, 6 for sunday */
	public int startWeekday = 6;
	
	/* elements padding */
	public int padding = 1;
	
	/* cells border properties */
	public int borderWidth = 1;
	public int borderColor = 0x0000ff;
	
	/* weekday labels properties */
	public Font weekdayFont = Font.getDefaultFont();
	public int weekdayBgColor = 0x0000ff;
	public int weekdayColor = 0xffffff;
	
	/* header (month-year label) properties */
	public Font headerFont = Font.getDefaultFont();
	public int headerBgColor = 0x0000ff;
	public int headerColor = 0xffffff;
	
	/* cells properties */
	public Font font = Font.getDefaultFont();
	public int foreColor = 0x000000;
	public int bgColor = 0x9999ff;
	public int selectedBgColor = 0xffff00;
	public int selectedForeColor = 0xff0000;
	
	/* internal properties */	
	int headerHeight = 0;
	int weekHeight = 0;
	
	
	/* internal time properties */
	long currentTimestamp = 0;
	Calendar calendar = null;
	int weeks = 0;
	
public CalendarWidget(Date date)
{
	calendar = Calendar.getInstance();
	
	//we'll see these 2 methods later
	setDate(date);
	
	initialize();
}
public Date getSelectedDate()
{
	return calendar.getTime();
}
public void setDate(Date d)
{
	currentTimestamp = d.getTime();
	
	calendar.setTime(d);
	
	//weeks number can change, depending on week starting day and month total days
	this.weeks = (int)Math.ceil(((double)getStartWeekday() + getMonthDays()) / 7);
}
public void setDate(long timestamp)
{
	setDate(new Date(timestamp));
}
void initialize()
{
	this.headerHeight = headerFont.getHeight() + 2 * padding;
	this.weekHeight = weekdayFont.getHeight() + 2 * padding;
}

int getMonthDays()
{
	int month = calendar.get(Calendar.MONTH);
	
	switch(month)
	{
	case 3:
	case 5:
	case 8:
	case 10:
		return 30;
	case 1:
//                Calendar c = Calendar.getInstance();	
//                c.set(Calendar.MONTH, 1);
//                c.set(Calendar.YEAR, calendar.get(Calendar.YEAR));
//                c.set(Calendar.DAY_OF_MONTH, 1);
//                return c.getActualMaximum(Calendar.DAY_OF_MONTH);
                    return calendar.get(Calendar.YEAR) % 4 == 0 && calendar.get(Calendar.YEAR) % 100 != 0 ? 29 : 28;
	default:
		return 31;
	}
}
int getStartWeekday()
{
	//let's create a new calendar with same month and year, but with day 1
	Calendar c = Calendar.getInstance();
	
	c.set(Calendar.MONTH, calendar.get(Calendar.MONTH));
	c.set(Calendar.YEAR, calendar.get(Calendar.YEAR));
	c.set(Calendar.DAY_OF_MONTH, 1);
	
	//we must normalize DAY_OF_WEEK returned value
	return (c.get(Calendar.DAY_OF_WEEK) + 5) % 7;
}
public void keyPressed(int key)
{
	switch(key)
	{
	case Canvas.UP:
		go(-7);
		break;
	case Canvas.DOWN:
		go(7);
		break;
	case Canvas.RIGHT:
		go(1);
		break;
	case Canvas.LEFT:
		go(-1);
		break;
	}
}
void go(int delta)
{
	int prevMonth = calendar.get(Calendar.MONTH);
	
	setDate(currentTimestamp + 86400000 * delta);

	//we have to check if month has changed
	//if yes, we have to recalculate month height
	//since weeks number could be changed
	if(calendar.get(Calendar.MONTH) != prevMonth)
	{
		
	}
}

public void DrawCalendar(Graphics g, int width, int height)
{ 
    //painting background
    g.setColor(bgColor);
    g.fillRect(0, 0, width, height);
    
    //painting header (month-year label)
    g.setFont(headerFont);
    g.setColor(headerColor);
    g.drawString(MONTH_LABELS[calendar.get(Calendar.MONTH)] + " " + calendar.get(Calendar.YEAR), width / 2, padding, Graphics.TOP | Graphics.HCENTER);
    
    //painting week days labels
    g.translate(0, headerHeight);

    g.setColor(weekdayBgColor);
    g.fillRect(0, 0, width, weekHeight);
    
    g.setColor(weekdayColor);
    g.setFont(weekdayFont);
    
    int cellWidth = (int)Math.ceil((double)(width - 8 * borderWidth) /7);
    for(int i = 0; i < 7; i++)
    {
            if(((i + startWeekday) % 7) == 6) g.setColor(0xFF0000);
            g.drawString(WEEKDAY_LABELS[(i + startWeekday) % 7],
                    borderWidth + i * (cellWidth + borderWidth) + cellWidth / 2,
                    padding,
                    Graphics.TOP | Graphics.HCENTER
            );
            if(((i + startWeekday) % 7) == 6) g.setColor(weekdayColor);
    }
    
    //painting cells borders
    g.translate(0, weekHeight);   
    
    int cellHeight = (int)Math.ceil((double)(height - headerHeight - weekHeight - (weeks+1) * borderWidth) / weeks);
    for(int i = 0; i <= weeks; i++)
    {
            g.fillRect(0, i * (cellHeight + borderWidth), width, borderWidth);
    }
    for(int i = 0; i <= 7; i++)
    {
            g.fillRect(i * (cellWidth + borderWidth), 0, borderWidth, height - headerHeight - weekHeight);
    }
    //painting days
    int days = getMonthDays();
    int dayIndex = (getStartWeekday() - this.startWeekday + 7) % 7;

    g.setColor(foreColor);

    int currentDay = calendar.get(Calendar.DAY_OF_MONTH);

    for(int i = 0; i < days; i++)
    {
            int weekday = (dayIndex + i) % 7;
            int row = (dayIndex + i) / 7;

            int x = borderWidth + weekday * (cellWidth + borderWidth) + cellWidth / 2;
            int y = borderWidth + row * (cellHeight + borderWidth) + padding;

            if(((weekday + startWeekday) % 7) == 6) g.setColor(0xFF0000);
            
            //if this is the current day, we'll use selected bg and fore colors
            if(i + 1 == currentDay)
            {
                    g.setColor(selectedBgColor);
                    g.fillRect(
                            borderWidth + weekday * (cellWidth + borderWidth), 
                            borderWidth + row * (cellHeight + borderWidth), 
                            cellWidth, cellHeight);
                    g.setColor(selectedForeColor);
            }

            g.drawString("" + (i + 1), x, y + (cellHeight - borderWidth - font.getHeight())/ 2 , Graphics.HCENTER | Graphics.TOP);

            //if this is the current day, we must restore standard fore color 
            if(i + 1 == currentDay || ((weekday + startWeekday) % 7) == 6)
            {
                    g.setColor(foreColor);
            }
    }
    //let's traslate back!
    g.translate(-0, - headerHeight - weekHeight);
}
}

