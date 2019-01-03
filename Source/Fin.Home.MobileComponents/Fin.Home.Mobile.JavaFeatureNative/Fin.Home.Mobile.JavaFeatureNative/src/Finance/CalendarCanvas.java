//<editor-fold defaultstate="collapsed" desc="comment">
package Finance;
//</editor-fold>

import java.util.Date;
import java.util.Vector;
import javax.microedition.io.Connector;
import javax.microedition.lcdui.*;
import javax.microedition.midlet.MIDlet;

public class CalendarCanvas extends Canvas implements CommandListener , ConfirmMsgBox.IConfirmRequester
{

    private Command exitCmd, listCmd, importCmd, deleteAllCmd;
    
    CalendarWidget calendar = null;
    FinanceMIDlet midlet = null;    
    
     public void commandAction(Command c, Displayable s)
     {
        if (c == listCmd)
        {
            midlet.ShowExpenseList(); 
            midlet.GetExpenseListDisp().SelectedDate = calendar.getSelectedDate();                    
            midlet.GetExpenseListDisp().Update();
        }
        else if(c == exitCmd)
        {
            midlet.exitMIDlet();
        }
        else if(c == deleteAllCmd)
        {
            midlet.ShowConfirmBoxDisp(this, "Delete All?",null);
        }
        else if(c == importCmd)
        {
            Vector recs = FinanceRecordStore.GetFinanceRecordsForExport();
            String ret = FinanceRecordStore.ImportFinanceRecords(recs).trim();
            boolean succ = ret.compareTo("") == 0;
            midlet.getDisplay().setCurrent(new Alert(succ ? "Export Successful" : ret),this);
        }
    }
     
    public CalendarCanvas(final FinanceMIDlet m) {      
       
        addCommand(listCmd = new Command("List", Command.SCREEN, 1));
        addCommand(exitCmd = new Command("Exit", Command.SCREEN, 4));
        addCommand(importCmd = new Command("Export All", Command.SCREEN, 2));
        addCommand(deleteAllCmd = new Command("Delete All", Command.SCREEN, 3));
        setCommandListener(this);
        
        this.midlet = m;

        calendar = new CalendarWidget(new Date());

        calendar.headerFont = Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_BOLD, Font.SIZE_LARGE);
        calendar.weekdayFont = Font.getFont(Font.FACE_PROPORTIONAL, Font.STYLE_BOLD, Font.SIZE_MEDIUM);
        calendar.weekdayBgColor = 0xccccff;
        calendar.weekdayColor = 0x0000ff;
        calendar.headerColor = 0xffffff;

        calendar.initialize();
    }

    protected void keyPressed(int key) {
        int keyCode = getGameAction(key);

        if (keyCode == FIRE) {
            commandAction(listCmd, this);
                       
        } else {
            calendar.keyPressed(keyCode);

            repaint();
        }
    }

    protected void paint(Graphics g) {
        calendar.DrawCalendar(g, getWidth(), getHeight());
    }
    
    public void ConfirmAction(int commandAction, Object RequesterObj)
    {
        if(commandAction == Command.OK)
        {
            FinanceRecordStore.RemoveFinanceStore();
        }
    }
}
