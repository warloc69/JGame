
using System;
using System.Text.RegularExpressions;

using UnityEditor;
using UnityEngine;
using UnityEngine.UI;

public class InputFieldUI : MonoBehaviour
{
	public void onValueChanged(string fieldName)
	{
		InputField field = GameObject.Find(fieldName).GetComponent<InputField>() as InputField;
		if(field != null)
		{
			string text = field.text.Trim();
			field.text = Regex.Replace(text, "[^a-zA-Z0-9]", "");
		}
	}

	public void onLogin()
	{
		InputField loginField = GameObject.Find("LoginInputField").GetComponent<InputField>() as InputField;
		InputField passField = GameObject.Find("PassInputField").GetComponent<InputField>() as InputField;
		if(loginField == null || passField == null)
			return;

		if(loginField.text.Length == 0)
		{
			showWarningPopup("Enter login");
			return;
		}

		if(passField.text.Length == 0)
		{
			showWarningPopup("Enter password");
			return;
		}

		Packet p = PacketBuilder.authRequestPacket (loginField.text, passField.text, 0);
        NetManager.sendAuthorizationPacket(p);
	}

	public void onRegister()
	{
		InputField loginField = GameObject.Find("LoginInputField").GetComponent<InputField>() as InputField;
		InputField passField = GameObject.Find("PassInputField").GetComponent<InputField>() as InputField;
		if(loginField == null || passField == null)
			return;
		
		if(loginField.text.Length == 0)
		{
			showWarningPopup("Enter login");
			return;
		}
		
		if(passField.text.Length == 0)
		{
			showWarningPopup("Enter password");
			return;
		}

		Packet p = PacketBuilder.authRequestPacket (loginField.text, passField.text, 1);
        NetManager.sendAuthorizationPacket(p);
	}
	
	public static void showWarningPopup(string text)
	{
		EditorUtility.DisplayDialog("WARNING", text, "OK");
	}
}
