package org.zmc.qutils;

import android.app.DialogFragment;
import android.app.Dialog;
import android.os.Bundle;

import android.app.TimePickerDialog;
import android.widget.TimePicker;
import android.content.DialogInterface;

import java.util.Calendar;
import java.text.DateFormat;

// qutils
import org.zmc.qutils.notification.CppCallbacks;
import org.zmc.qutils.QutilsActivity;

public class TimePickerFragment extends DialogFragment implements TimePickerDialog.OnTimeSetListener {

    private static QutilsActivity m_MainContext;

    public TimePickerFragment(QutilsActivity mainActivity)
    {
        m_MainContext = mainActivity;
    }


    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        // Use the current time as the default values for the picker
        final Calendar c = Calendar.getInstance();
        int hour = c.get(Calendar.HOUR_OF_DAY);
        int minute = c.get(Calendar.MINUTE);

        // Create a new instance of TimePickerDialog and return it
        return new TimePickerDialog(m_MainContext, this, hour, minute, true);
    }

    public void onTimeSet(TimePicker view, int hourOfDay, int minute) {
        CppCallbacks.timePicked(hourOfDay, minute);
    }

    @Override
    public void onCancel(DialogInterface dialog) {
        CppCallbacks.timePicked(-1, -1);
    }
}
