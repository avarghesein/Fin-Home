//<editor-fold defaultstate="collapsed" desc="comment">
package Finance;
//</editor-fold>

import java.util.Date;
import java.util.*;
import javax.microedition.lcdui.*;
import javax.microedition.midlet.MIDlet;
import java.util.Calendar;

/**
 *
 * @author Abraham
 */
public class ExpenseList extends List implements CommandListener, ConfirmMsgBox.IConfirmRequester
{
    private Command addCmd, delCmd, bkCmd;
    
    public void commandAction(Command c, Displayable s)
    {
        if (c == bkCmd)
        {
            midlet.ShowCalendar();
        }
        else if(c == delCmd)
        {
            midlet.ShowConfirmBoxDisp(this, "Delete selected records?",null);
        }
        else if(c == addCmd)
        {
            midlet.ShowxpenseEntryDisp();
        }
    }
    
    FinanceMIDlet midlet = null;
    
    public Date SelectedDate;
    
    public void Update()
    { 
        setTitle("Expenses: " + CalendarWidget.GetDateString(SelectedDate));
        FillExpenses();
    }
    
    Vector _expenseItems;
    
    public void FillExpenses()
    {
        deleteAll();
        
        _expenseItems = null;
        _expenseItems = FinanceRecordStore.GetFinanceRecords(CalendarWidget.GetRecordStoreDateString(midlet.GetExpenseListDisp().SelectedDate));
        if(_expenseItems == null) return;
        for(int i = 0; i < _expenseItems.size(); ++ i) 
        {
            insert(i, ((FinanceRecord)_expenseItems.elementAt(i)).RecordDetails, null);
        }
    }
    
    public ExpenseList(final FinanceMIDlet m)
    {   
        super("Expenses",Choice.MULTIPLE);
        
        this.midlet = m;      
        
        addCommand(addCmd = new Command("Add", Command.SCREEN, 1));
        addCommand(bkCmd = new Command("Back", Command.SCREEN, 2));        
        addCommand(delCmd = new Command("Delete", Command.SCREEN, 3));
        setCommandListener(this);        
    }
    
    public void ConfirmAction(int commandAction, Object RequesterObj)
    {
        if(commandAction == Command.OK)
        {
            DeleteSelected();
        }
    }
    
    public void DeleteSelected()
    {
        for(int idx = 0; idx < size(); ++idx)
            if(isSelected(idx))
            {
                FinanceRecordStore.DeleteFinanceRecord((FinanceRecord)_expenseItems.elementAt(idx));
            }
        
        Update();
    }
}
