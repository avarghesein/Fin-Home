/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Finance;

import javax.microedition.lcdui.*;

public class ConfirmMsgBox extends Canvas implements CommandListener
{
    public interface IConfirmRequester
    {
        void ConfirmAction(int commandAction, Object RequesterObj);
    };
    
    private Command okCmd, cancelCmd;
    
    FinanceMIDlet midlet = null;

     public void commandAction(Command c, Displayable s)
     {
        midlet.switchDisplayable(null, (Displayable)_source);
        _source.ConfirmAction(c.getCommandType(), _requesterObj);
    }
     
    public ConfirmMsgBox(final FinanceMIDlet m) {
        
        addCommand(okCmd = new Command("Yes", Command.OK, 2));
        addCommand(cancelCmd = new Command("No", Command.CANCEL, 1));
        setCommandListener(this);
        
        this.midlet = m;
    }
    
    IConfirmRequester _source;
    String _message;
    Object _requesterObj;
    
    public void ShowConfirmBox(IConfirmRequester source,String message, Object requesterObj)
    {
        if(source == null) return;
        _source = source;
        _message = message;
        _requesterObj = requesterObj;
        midlet.switchDisplayable(null, this);
        repaint();        
    }
    
    protected void paint(Graphics g)
    {
        Font font = Font.getDefaultFont();
        int gWidth = getWidth();
        int gHeight = getHeight();
        
        g.setFont(font);
        g.setColor(0x0000ff);
        g.fillRect(0, 0, gWidth, gHeight);
        g.setColor(0xffffff);        
        g.drawString(_message, gWidth/2, gHeight/2, Graphics.TOP | Graphics.HCENTER);
    }
    
    protected void keyPressed(int key) {
        int keyCode = getGameAction(key);

        if (keyCode == FIRE)
        {
            commandAction(cancelCmd, this);
        }
    }
};
