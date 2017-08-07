package org.zmc.qutils;

import android.app.DialogFragment;
import android.app.Dialog;
import android.os.Bundle;

import android.app.DatePickerDialog;
import android.widget.DatePicker;
import android.content.DialogInterface;

import java.util.Calendar;

// qutils
import org.zmc.qutils.CppCallbacks;
import org.zmc.qutils.QutilsActivity;

public class DatePickerFragment extends DialogFragment implements DatePickerDialog.OnDateSetListener {

    private static QutilsActivity m_MainContext;

    public DatePickerFragment(QutilsActivity mainActivity) {
        m_MainContext = mainActivity;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        // Use the current date as the default date in the picker
        final Calendar c = Calendar.getInstance();
        int year = c.get(Calendar.YEAR);
        int month = c.get(Calendar.MONTH);
        int day = c.get(Calendar.DAY_OF_MONTH);

        // Create a new instance of DatePickerDialog and return it
        DatePickerDialog dialog = new DatePickerDialog(m_MainContext, this, year, month, day);
        return dialog;
    }

    public void onDateSet(DatePicker view, int year, int month, int day) {
        CppCallbacks.datePicked(year, month, day);
    }

    @Override
    public void onCancel(DialogInterface dialog) {
        CppCallbacks.datePicked(-1, -1, -1);
    }
}
