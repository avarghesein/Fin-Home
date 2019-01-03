
//!-- Common Functions -->

function GetDBPath() {

    return CurrentPath() + "\\Finance@Home_DB.mdb";
    //return "C:/ABRAHAM/Test/FinanceHome/Finance@Home_DB.mdb";
}

function OpenConnection() {

    var con = new ActiveXObject('ADODB.Connection');
    con.open('Provider=Microsoft.Jet.OLEDB.4.0;Data Source=' + GetDBPath() + ';');
    return con;
}

function GetItem(item) {
    return document.getElementById(item);
}

function Quotient(numerator, denominator) {
    // In JavaScript, dividing integer values yields a floating point result (unlike in Java, C++, C)
    // To find the integer quotient, reduce the numerator by the remainder first, then divide.
    var remainder = numerator % denominator;
    var quotient = (numerator - remainder) / denominator;

    // Another possible solution: Convert quotient to an integer by truncating toward 0.
    // Thanks to Frans Janssens for pointing out that the floor function is not correct for negative quotients.
    /****************************************************
    if ( quotient >= 0 )
    quotient = Math.floor( quotient );
    else  // negative
    quotient = Math.ceil( quotient );
    *****************************************************/

    return quotient;
}

function SelectListItemByText(listControl, itemText) {
    for (var i = 0; i < listControl.options.length; ++i) {
        if (listControl.options[i].text == itemText) {
            listControl.options[i].selected = true;
            return;
        }
    }
}

function GetSelectedListItemValue(listControl) {
    for (var i = 0; i < listControl.options.length; ++i) {
        if (listControl.options[i].selected == true) {
            return listControl.options[i].value;
        }
    }
}

function AppendListItem(listControl, itemText, itemVal) {
    var elOptNew = document.createElement('option');
    elOptNew.text = itemText;
    elOptNew.value = itemVal;
    var elSel = listControl;

    try {
        elSel.add(elOptNew, null); // standards compliant; doesn't work in IE
    }
    catch (ex) {
        elSel.add(elOptNew); // IE only
    }
}

function CurrentPath() {
    var path = document.location.href;
    path = path.replace(/\\/g, "/");
    path = path.substr(5, path.length - 5);
    path = path.substr(0, path.lastIndexOf("/") + 1);
    path = path.replace(/\//g, '\\');
    path = path.replace(/%20/g, ' ');

    path = path.replace(/^[\\]+(.*)$/, '$1'); // Trims leading spaces
    path = path.replace(/^(.*)[\\]+$/, '$1'); // Trims trailing spaces

    return path;
}

function CloseConnection(cn) {
    cn.Close();
}


function OpenCommand(cn, sql) {

    cn.Execute(sql);
}

function OpenRecordset(cn, sql) {

    var rs = new ActiveXObject('ADODB.Recordset');
    rs = cn.Execute(sql);
    return rs;
}

function SendTABkeyOnEnter(e) {
    // look for window.event in case event isn't passed in
    if (window.event) { e = window.event; }
    if (e.keyCode == 13) {
        event.keyCode = 9;
        return event.keyCode;
    }
}

//!-- DB Functions -->

function DebitCreditRecord() {
    this.Date = new Date();
    this.ItemDesc = "";
    this.Amount = 0;
    this.IsDebit = true;
    this.CategoryCode = "";
    this.ItemKey = 0;
    this.OneTime = false;
    this.BookMark = false;
}

function UpdateOneTimeAndBookMark(item, connection) {
    var cmd = "DELETE FROM DEBITS_CREDITS_BOOKMARK WHERE TIME_STAMP=" + item.ItemKey;
    OpenCommand(connection, cmd);

    cmd = "DELETE FROM DEBITS_CREDITS_ONETIME WHERE TIME_STAMP=" + item.ItemKey;
    OpenCommand(connection, cmd);

    if (item.OneTime == true) {
        cmd = "INSERT INTO DEBITS_CREDITS_ONETIME(TIME_STAMP,IS_ONETIME) VALUES(" + item.ItemKey + ",true)";
        OpenCommand(connection, cmd);
    }

    if (item.BookMark == true) {
        cmd = "INSERT INTO DEBITS_CREDITS_BOOKMARK(TIME_STAMP,IS_MARKED) VALUES(" + item.ItemKey + ",true)";
        OpenCommand(connection, cmd);
    }
}

function Add(item) {

    var con = OpenConnection();
    try {

        var cmd = "INSERT INTO DEBITS_CREDITS([DATE],TRANS_DESC,AMOUNT,IS_DEBIT,TIME_STAMP,CATEGORY_CODE) " +
        "VALUES(#" + (item.Date.getMonth() + 1) + "/" + item.Date.getDate() + "/" + item.Date.getFullYear() + "#" +
        ",'" + item.ItemDesc + "'," + item.Amount + "," + item.IsDebit + "," +
        item.ItemKey + ",'" + item.CategoryCode + "')";

        OpenCommand(con, cmd);
        UpdateOneTimeAndBookMark(item, con);
    }
    catch (e) {
        alert(e);
    }
    CloseConnection(con);
    Init(GetItem('tabDebits'));
}

function Update(item) {
    var con = OpenConnection();
    try {

        var cmd = "UPDATE DEBITS_CREDITS SET " +
        "[DATE] = #" + (item.Date.getMonth() + 1) + "/" + item.Date.getDate() + "/" + item.Date.getFullYear() + "#, " +
        "TRANS_DESC = '" + item.ItemDesc + "', AMOUNT = " + item.Amount + ", IS_DEBIT=" + item.IsDebit + ", " +
         "CATEGORY_CODE = '" + item.CategoryCode + "' WHERE TIME_STAMP=" + item.ItemKey;

        OpenCommand(con, cmd);
        UpdateOneTimeAndBookMark(item, con);
    }
    catch (e) {
        alert(e);
    }
    CloseConnection(con);
    Init(GetItem('tabDebits'));
}

function Delete(itemKey) {
    var con = OpenConnection();
    try {

        var cmd = "DELETE FROM DEBITS_CREDITS_BOOKMARK WHERE TIME_STAMP=" + itemKey;
        OpenCommand(con, cmd);

        cmd = "DELETE FROM DEBITS_CREDITS_ONETIME WHERE TIME_STAMP=" + itemKey;
        OpenCommand(con, cmd);

        cmd = "DELETE FROM DEBITS_CREDITS WHERE TIME_STAMP=" + itemKey;
        OpenCommand(con, cmd);
    }
    catch (e) {
        alert(e);
    }
    CloseConnection(con);
    Init(GetItem('tabDebits'));
}

function LoadCategories(listCategories) {
    listCategories.innerHTML = "";

    var con = OpenConnection();
    var rs = OpenRecordset(con,
        "SELECT CAT_CODE,CAT_NAME " +
        "FROM CATEGORY_MASTER ORDER BY CAT_NAME");

    if (rs.EOF) {
        return;
    }

    rs.MoveFirst();
    while (!rs.EOF) {
        AppendListItem(listCategories, rs.Fields("CAT_NAME").Value, rs.Fields("CAT_CODE").Value);
        rs.MoveNext();
    }
    con.Close();
}

function Init(control) {
    GetItem('spnAmount').innerHTML = "";
    control.innerHTML = "Searching....";

    var con = OpenConnection();

    var whereCond = "WHERE(";

    if (GetItem('dteDateSrch').value == null || GetItem('dteDateSrch').value == "") {

    }
    else {

        var dteSrch = new Date(GetItem('dteDateSrch').value);
        whereCond += " DATE >= #" + (dteSrch.getMonth() + 1) + "/" + dteSrch.getDate() + "/" + dteSrch.getFullYear() + "# AND ";
    }

    if (GetItem('dteDateSrchEnd').value == null || GetItem('dteDateSrchEnd').value == "") {
    }
    else {

        var dteSrch = new Date(GetItem('dteDateSrchEnd').value);
        whereCond += " DATE <= #" + (dteSrch.getMonth() + 1) + "/" + dteSrch.getDate() + "/" + dteSrch.getFullYear() + "# AND ";
    }

    if (GetItem('chkDebit').checked == false) {
        whereCond += " IS_DEBIT = False AND ";
    }
    else {

        whereCond += " IS_DEBIT = True AND ";
    }

    if (GetItem('chkMarkedOnly').checked == true) {

        whereCond += " IS_MARKED = True AND ";
    }

    if (GetItem('chkOnetimeOnly').checked == true) {

        whereCond += " IS_ONETIME = True AND ";
    }

    whereCond += " 1 = 1 )";

    var rs = OpenRecordset(con,
        "SELECT IS_ONETIME,IS_MARKED,DATE,TRANS_DESC,AMOUNT,IS_DEBIT,DEBITS_CREDITS.TIME_STAMP,CAT_NAME " +
        "FROM ((DEBITS_CREDITS LEFT JOIN CATEGORY_MASTER ON " +
        "DEBITS_CREDITS.CATEGORY_CODE = CATEGORY_MASTER.CAT_CODE) " +
        "LEFT JOIN DEBITS_CREDITS_BOOKMARK ON DEBITS_CREDITS_BOOKMARK.TIME_STAMP=DEBITS_CREDITS.TIME_STAMP) " +
        "LEFT JOIN DEBITS_CREDITS_ONETIME ON DEBITS_CREDITS_ONETIME.TIME_STAMP=DEBITS_CREDITS.TIME_STAMP " +
        whereCond +
        " ORDER BY DATE DESC");


    var data = "<table id='tblDebitCreditList' border='1px' cellspacing='1' style='text-align:left;width:100%'>" +
    "<th>Date</th>" +
    "<th>Item</th>" +
    "<th>Amount</th>" +
    "<th>Debit?</th>" +
    "<th>Category</th>" +
     "<th>Details</th>"

    if (rs.EOF) {
        control.innerHTML = "<span style='align:center'>NO DATA!</span>";
        return;
    }
    var clr = "odd";
    var totAmount = 0;

    rs.MoveFirst();
    while (!rs.EOF) {
        var dt = new Date(rs.Fields("DATE").Value);

        clr = clr == "odd" ? "evn" : "odd";

        var attribs = "";

        attribs += " IS_ONETIME=" + rs.Fields("IS_ONETIME").Value + " IS_MARKED=" + rs.Fields("IS_MARKED").Value + " TIME_STAMP=" + rs.Fields("TIME_STAMP").Value;

        var dtls = "";

        if (("" + rs.Fields("IS_MARKED").Value) == "true") dtls += "Marked;";
        if (("" + rs.Fields("IS_ONETIME").Value) == "true") dtls += "OneTime;";

        if (dtls == "") dtls = "N/A";

        data += "<tr class='" + clr + "' onkeyup='HandleTableArrowKeys(event)' onclick='Toggle(this)'>" +
        "<td>" + (dt.getMonth() + 1) + "/" + dt.getDate() + "/" + dt.getFullYear() + "</td>" +
        "<td style='text-align:left;width:50%;'>" + rs.Fields("TRANS_DESC").Value + "</td>" +
        "<td>" + rs.Fields("AMOUNT").Value + "</td>" +
        "<td>" + rs.Fields("IS_DEBIT").Value + "</td>" +
        "<td>" + rs.Fields("CAT_NAME").Value + "</td>" +
        "<td " + attribs + ">" + dtls + "</td>" +
        "</tr>"

        totAmount += parseFloat(rs.Fields("AMOUNT").Value);

        rs.MoveNext();
    }
    prevRow = null;
    control.innerHTML = data + "</table>";

    var t = new ScrollableTable(document.getElementById('tblDebitCreditList'), 550, 690);
    GetItem('spnAmount').innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;<b>RS/- " + totAmount + "</b>";
    con.Close();
}

function HandleTableArrowKeys(evt) {
    evt = (evt) ? evt : ((window.event) ? event : null);
    if (evt && prevRow != null) {
        var prevIndex = prevRow.rowIndex;
        if (GetItem('tblDebitCreditList') != null) {
            var rowLength = GetItem('tblDebitCreditList').rows.length;
            if (rowLength >= 2) {
                switch (evt.keyCode) {
                    case 38:
                        prevIndex -= 1;
                        if (prevIndex <= 0) {
                            prevIndex = rowLength - 1;
                        }
                        break;
                    case 40:
                        prevIndex += 1;
                        if (prevIndex >= rowLength) {
                            prevIndex = 1;
                        }
                        break;
                }
                SelectNextRow(prevIndex);
            }
        }
    }
}


//UI Functins

var prevRow = null;
function Toggle(it) {
    if (it.className.substring(0, 3) == "sel") {
        it.className = it.className.substring(3, 6);
        prevRow = null;

        NewItem();
        GetItem('cboCategory').selectedIndex = -1;
    }
    else {
        it.className = "sel" + it.className;
        if (prevRow != null)
        { prevRow.className = prevRow.className.substring(3, 6); }
        prevRow = it;

        GetItem('dteDate').value = it.cells[0].innerHTML;
        GetItem('txtItem').value = it.cells[1].innerHTML;
        GetItem('txtAmount').value = it.cells[2].innerHTML;
        GetItem('chkDebitEdit').checked = it.cells[3].innerHTML == "true";
        SelectListItemByText(GetItem('cboCategory'), it.cells[4].innerHTML);
        GetItem('lblKey').innerHTML = it.cells[5].getAttribute("TIME_STAMP");
        GetItem('chkOneTime').checked = it.cells[5].getAttribute("IS_ONETIME") == "true";
        GetItem('chkBookMark').checked = it.cells[5].getAttribute("IS_MARKED") == "true";
    }
}

function SelectNextRow(curRowIndex) {
    if (GetItem('tblDebitCreditList') != null && curRowIndex >= 1) {

        var rowLength = GetItem('tblDebitCreditList').rows.length;
        if (rowLength > 1) {
            if (curRowIndex >= rowLength) curRowIndex = rowLength - 1;
            var row = GetItem('tblDebitCreditList').rows[curRowIndex];
            Toggle(row);
            row.focus();
        }
    }
}

function Load() {

    SetAlerts();
    LoadCategories(GetItem('cboCategory'));
    var dteSrch = new Date();
    GetItem('dteDateSrch').value = (dteSrch.getMonth() + 1) + "/" + dteSrch.getDate() + "/" + dteSrch.getFullYear();
    GetItem('dteDateSrchEnd').value = (dteSrch.getMonth() + 1) + "/" + dteSrch.getDate() + "/" + dteSrch.getFullYear();
    Init(GetItem('tabDebits'));

    new JsDatePick({
        useMode: 2,
        target: "dteDate",
        dateFormat: "%m/%d/%Y"
    });

    new JsDatePick({
        useMode: 2,
        target: "dteDateSrchEnd",
        dateFormat: "%m/%d/%Y"
    });

    new JsDatePick({
        useMode: 2,
        target: "dteDateSrch",
        dateFormat: "%m/%d/%Y"

    });

    NewItem();
}

function NewItem() {
    var now = new Date();
    GetItem('dteDate').value = (now.getMonth() + 1) + "/" + now.getDate() + "/" + now.getFullYear();
    GetItem('txtItem').value = "";
    GetItem('txtAmount').value = "";
    GetItem('chkDebitEdit').checked = true;
    //GetItem('cboCategory').selectedIndex = -1;
    GetItem('chkOneTime').checked = false;
    GetItem('chkBookMark').checked = false;
    GetItem('lblKey').innerHTML = "";
    GetItem('txtItem').focus();
}

function OnAdd() {
    var item = new DebitCreditRecord();
    var status = ValidateInputs(item, false);
    if (status == true) {
        Add(item);
        NewItem();
    }
}

function OnUpdate() {
    var item = new DebitCreditRecord();
    var status = ValidateInputs(item, true);
    if (status == true) {
        Update(item);
        NewItem();
    }
}

function OnDelete() {
    var itemKey = parseInt(GetItem('lblKey').innerHTML);
    if (isNaN(itemKey) == true || itemKey <= 0) {
        alert('Please select an entry from the list to delete');
        return;
    }
    if (confirm("Are you sure to delete this item?") == true) {
        var prevIndex = prevRow.rowIndex;
        Delete(itemKey);
        SelectNextRow(prevIndex);
        GetItem('txtItem').focus();
    }
}

function ValidateInputs(item, isUpdate) {

    try {

        item.Date = new Date(GetItem('dteDate').value);
        item.ItemDesc = GetItem('txtItem').value;
        item.Amount = parseFloat(GetItem('txtAmount').value);
        item.IsDebit = GetItem('chkDebitEdit').checked;
        item.OneTime = GetItem('chkOneTime').checked;
        item.BookMark = GetItem('chkBookMark').checked;
        item.CategoryCode = GetSelectedListItemValue(GetItem('cboCategory'));
        if (isUpdate == true) {
            item.ItemKey = parseInt(GetItem('lblKey').innerHTML);
        }
        else {
            var millennium = new Date("01/01/2000 12:00 AM");
            var today = new Date();
            var one_sec = 1000; //we need diff in seconds
            item.ItemKey = Quotient(today.getTime() - millennium.getTime(), one_sec);
            GetItem('lblKey').innerHTML = item.ItemKey;
        }

        var msg = "";
        if (item.Date == "NaN" || item.Date == "") {
            GetItem('dteDate').focus();
            msg += "Date is mandatory\n";
        }
        if (item.ItemDesc == "NaN" || item.ItemDesc == "") {
            GetItem('txtItem').focus();
            msg += "Item is mandatory\n";
        }
        if (isNaN(item.Amount) == true || item.Amount <= 0) {
            GetItem('txtAmount').focus();
            msg += "Amount is mandatory\n";
        }
        if (item.CategoryCode == "NaN" || item.CategoryCode <= 0) {
            GetItem('cboCategory').focus();
            msg += "Category is mandatory\n";
        }
        if (isNaN(item.ItemKey) == true || item.ItemKey <= 0) {
            msg += "TimeStamp is mandatory\n";
        }
        if (msg != "") {
            GetItem('lblKey').innerHTML = "";
            alert(msg);
        }
        return msg == "";
    }
    catch (e) {
        alert(e);
        return false;
    }
}

function LoadReports() {

    var cn = GetConnection();

    var polRpt = PolicyDueReport(cn);
    var rptElmt = GetItem("divPolRpt");
    rptElmt.innerHTML = polRpt;

    var invRpt = InvestmentReport(cn);
    rptElmt = GetItem("divInvRpt");
    rptElmt.innerHTML = invRpt;

    var tskRpt = TaskReport(cn);
    rptElmt = GetItem("divTskRpt");
    rptElmt.innerHTML = tskRpt;

    cn.Close();
}

function PolicyDueReport(cn) {

    var rpt = "";
    var rs;
    rs = qryPolicyDues(cn);
    if (!rs.EOF) {
        rs.Sort = "DUE_DATE ASC";

        rs.MoveFirst();

        rpt +=
        "<th>Status</th>" +
        "<th>By</th>" +
        "<th>Due Date</th>" +
        "<th>Due Premium</th>" +
        "<th>Policy Name</th>" +
        "<th>End Date</th>" +
        "<th>Return</th>";

        while (!rs.EOF) {

            var clr;
            var stat = Nz(Value(rs, "POL_STAT"), "");
            clr = StatusColor(stat);

            rpt += "<tr " + (clr == "" ? " style='font-weight:bold;' " : " style='font-weight:bold;color:" + clr + ";' ") + ">" +

            "<td>" + GetStatusString(Nz(Value(rs, "POL_STAT"), "")) + "</td>" +
            "<td>" + (Nz(Value(rs, "DUE_DATE"), "") == "" ?
                "N/A" :
                GetTimeUnitToGo(
                Value(rs, "DUE_DATE"),
                Nz(Value(rs, "STATUS"), "") != "ACTIVE")) + "</td>" +

            "<td>" + Format(Nz(Value(rs, "DUE_DATE"), ""), "mmm/dd/yyyy") + "</td>" +
            "<td>" + Value(rs, "DUE_PAY") + "</td>" +
            "<td>" + Value(rs, "POLICY_NAME") + "</td>" +
            "<td>" + Format(Value(rs, "END_DATE"), "mmm/dd/yyyy") + "</td>" +
            "<td>" + Value(rs, "POL_TOT_RETURN") + "</td>" +
            "</tr>";

            rs.MoveNext();
        }
        if (rpt != "") {
            rpt = "<table border='1px' cellspacing='1' style='text-align:left;width:100%'>" + rpt + "</table>";
        }
    }
    return rpt;
}

function InvestmentReport(cn) {

    var rpt = "";
    var rs;
    rs = qryInvestmentDetails(cn);
    if (!rs.EOF) {
        rs.Sort = "END_DATE ASC";

        rs.MoveFirst();

        rpt +=
        "<th>Alert Status</th>" +
        "<th>By</th>" +
        "<th>Due Date</th>" +
        "<th>Amount</th>" +
        "<th>Investment Name</th>" +
        "<th>%P.A</th>";

        while (!rs.EOF) {

            var clr;
            var stat = Nz(Value(rs, "INV_STATUS"), "");
            clr = StatusColor(stat);

            rpt += "<tr " + (clr == "" ? " style='font-weight:bold;' " : " style='font-weight:bold;color:" + clr + ";' ") + ">" +

            "<td>" + GetStatusString(Nz(Value(rs, "INV_STATUS"), "")) + "</td>" +
            "<td>" + (Nz(Value(rs, "END_DATE"), "") == "" ?
                "N/A" :
                GetTimeUnitToGo(
                Value(rs, "END_DATE"),
                false)) + "</td>" +

            "<td>" + Format(Nz(Value(rs, "END_DATE"), ""), "mmm/dd/yyyy") + "</td>" +
            "<td>" + Value(rs, "INV_AMOUNT") + "</td>" +
            "<td>" + Value(rs, "INVESTMENT_NAME") + "</td>" +
            "<td>" + Value(rs, "INTEREST_RATE") + "</td>" +
            "</tr>";

            rs.MoveNext();
        }
        if (rpt != "") {
            rpt = "<table border='1px' cellspacing='1' style='text-align:left;width:100%'>" + rpt + "</table>";
        }
    }
    return rpt;
}

function TaskReport(cn) {

    var rpt = "";
    var rs;
    rs = qryScheduledTaskStatus(cn);
    if (!rs.EOF) {
        rs.Sort = "TSK_TIME ASC";

        rs.MoveFirst();

        rpt +=
        "<th>Status</th>" +
        "<th>By</th>" +
        "<th>Date</th>" +
        "<th>Name</th>" +
        "<th>Next Date</th>" +
        "<th>Alert!</th>";

        while (!rs.EOF) {

            var clr;
            var stat = Nz(Value(rs, "STATUS"), "");
            clr = StatusColor(stat);

            rpt += "<tr " + (clr == "" ? " style='font-weight:bold;' " : " style='font-weight:bold;color:" + clr + ";' ") + ">" +

            "<td>" + GetStatusString(Nz(Value(rs, "STATUS"), "")) + "</td>" +
            "<td>" + GetTimeUnitToGo(
                Value(rs, "TSK_TIME"),
                Value(rs, "TSK_COMPLETED")) + "</td>" +

            "<td>" + Format(Nz(Value(rs, "TSK_TIME"), ""), "mmm/dd/yyyy") + "</td>" +
            "<td>" + Value(rs, "TSK_NAME") + "</td>" +

            "<td>" +

            ((Nz(Value(rs, "TSK_RECURRING_NEXT"), "") == "" || parseInt(Value(rs, "TSK_RECURRING_NEXT")) <= 0) ?
            "N/A"
            :
            Format(AddDateTimeBasedOnUnits(Value(rs, "TSK_RECURRING_NEXT"),
            Value(rs, "TSK_RECURRING_UNIT"), Value(rs, "TSK_TIME")), "mmm, dd yyyy")) + "</td>" +

            "<td>" + Value(rs, "TSK_ALERT") + "</td>" +
            "</tr>";

            rs.MoveNext();
        }
        if (rpt != "") {
            rpt = "<table border='1px' cellspacing='1' style='text-align:left;width:100%'>" + rpt + "</table>";
        }
    }
    return rpt;
}

function Value(rs, field) {
    return rs.Fields(field).Value;
}

function StatusColor(status) {
    var clr = "";
    switch (status) {
        case "ND":
            clr = "rgb(255, 165, 0)";
            break;

        case "AD":
            clr = "rgb(234, 106, 215)";
            break;

        case "D":
        case "TD":
            clr = "rgb(200, 0, 0)";
            break;

        case "C":
            clr = "rgb(0, 200, 0)";
            break;
    }
    return clr;
}

