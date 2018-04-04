/* poppler-form.h: qt interface to poppler
 * Copyright (C) 2007-2008, Pino Toscano <pino@kde.org>
 * Copyright (C) 2008, 2011, 2016, 2017, Albert Astals Cid <aacid@kde.org>
 * Copyright (C) 2012, Adam Reichold <adamreichold@myopera.com>
 * Copyright (C) 2016, Hanno Meyer-Thurow <h.mth@web.de>
 * Copyright (C) 2017, Hans-Ulrich Jüttner <huj@froreich-bioscientia.de>
 * Copyright (C) 2017, Tobias C. Berner <tcberner@freebsd.org>
 * Copyright (C) 2018, Andre Heinecke <aheinecke@intevation.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef _POPPLER_QT5_FORM_H_
#define _POPPLER_QT5_FORM_H_

#include <time.h>
#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtCore/QRectF>
#include <QtCore/QStringList>
#include <QtCore/QSharedPointer>
#include "poppler-export.h"

class Page;
class FormWidget;
class FormWidgetButton;
class FormWidgetText;
class FormWidgetChoice;
class FormWidgetSignature;

namespace Poppler {

    class DocumentData;
    class Link;

    class FormFieldData;
    /**
      The base class representing a form field.

      \since 0.6
     */
    class POPPLER_QT5_EXPORT FormField {
    public:

	/**
	   The different types of form field.
	*/
	enum FormType {
	    FormButton,    ///< A button field. See \ref Poppler::FormFieldButton::ButtonType "ButtonType"
	    FormText,      ///< A text field. See \ref Poppler::FormFieldText::TextType "TextType"
	    FormChoice,    ///< A single choice field. See \ref Poppler::FormFieldChoice::ChoiceType "ChoiceType"
	    FormSignature  ///< A signature field.
	};

	virtual ~FormField();

	/**
	  The type of the field.
	 */
	virtual FormType type() const = 0;

	/**
	   \return The size of the field, in normalized coordinates, i.e.
	   [0..1] with regard to the dimensions (cropbox) of the page
	*/
	QRectF rect() const;

	/**
	  The ID of the field.
	 */
	int id() const;

	/**
	  The internal name (T) of the field.
	 */
	QString name() const;

        /**
	  Sets the internal name (T) of the field.
	  \since 0.51
	 */
	void setName(const QString &name) const;
	
	/**
	  The internal fully qualified name of the field.
	  \since 0.18
	 */
	QString fullyQualifiedName() const;

	/**
	  The name of the field to be used in user interface (eg messages to
	  the user).
	 */
	QString uiName() const;

	/**
	  Whether this form field is read-only.
	 */
	bool isReadOnly() const;

	/**
	  Set whether this form field is read-only.
	  \since 0.64
	 */
	void setReadOnly(bool value);

	/**
	  Whether this form field is visible.
	 */
	bool isVisible() const;

	/**
	  The activation action of this form field.

	  \note It may be null.
	 */
	Link* activationAction() const;

        /**
	  * Describes the flags from the form 'AA' dictionary.
	  *
	  * \since 0.53
	 */
	enum AdditionalActionType
	{
	    FieldModified,   ///< A JavaScript action to be performed when the user modifies the field
	    FormatField,     ///< A JavaScript action to be performed before the field is formatted to display its value
	    ValidateField,   ///< A JavaScript action to be performed when the field value changes
	    CalculateField,  ///< A JavaScript action to be performed when the field needs to be recalculated
	};
	/**
	  * Returns a given form additional action
	  *
	  * \since 0.53
	 */
	Link* additionalAction(AdditionalActionType type) const;

    protected:
	/// \cond PRIVATE
	FormField(FormFieldData &dd);

	FormFieldData *m_formData;
	/// \endcond

    private:
	Q_DISABLE_COPY(FormField)
    };

    /**
      A form field that represents a "button".

      \since 0.8
     */
    class POPPLER_QT5_EXPORT FormFieldButton : public FormField {
    public:

	/**
	 * The types of button field.
	 */
	enum ButtonType
	{
	    Push,          ///< A simple push button.
	    CheckBox,      ///< A check box.
	    Radio          ///< A radio button.
	};

	/// \cond PRIVATE
	FormFieldButton(DocumentData *doc, ::Page *p, ::FormWidgetButton *w);
	/// \endcond
	~FormFieldButton();

	FormType type() const override;

	/**
	  The particular type of the button field.
	 */
	ButtonType buttonType() const;

	/**
	 * The caption to be used for the button.
	 */
	QString caption() const;

	/**
	  The state of the button.
	 */
	bool state() const;

	/**
	  Sets the state of the button to the new \p state .
	 */
	void setState( bool state );

	/**
	  The list with the IDs of siblings (ie, buttons belonging to the same
	  group as the current one.

	  Valid only for \ref Radio buttons, an empty list otherwise.
	 */
	QList<int> siblings() const;

    private:
	Q_DISABLE_COPY(FormFieldButton)
    };

    /**
      A form field that represents a text input.

      \since 0.6
     */
    class POPPLER_QT5_EXPORT FormFieldText : public FormField {
    public:

	/**
	   The particular type of this text field.
	*/
	enum TextType {
	    Normal,        ///< A simple singleline text field.
	    Multiline,     ///< A multiline text field.
	    FileSelect     ///< An input field to select the path of a file on disk.
	};

	/// \cond PRIVATE
	FormFieldText(DocumentData *doc, ::Page *p, ::FormWidgetText *w);
	/// \endcond
	~FormFieldText();

	FormType type() const override;

	/**
	  The text type of the text field.
	 */
	TextType textType() const;

	/**
	  The text associated with the text field.
	 */
	QString text() const;

	/**
	  Sets the text associated with the text field to the specified
	  \p text.
	 */
	void setText( const QString& text );

	/**
	  Whether this text field is a password input, eg its text \b must be
	  replaced with asterisks.

	  Always false for \ref FileSelect text fields.
	 */
	bool isPassword() const;

	/**
	  Whether this text field should allow rich text.
	 */
	bool isRichText() const;

	/**
	  The maximum length for the text of this field, or -1 if not set.
	 */
	int maximumLength() const;

	/**
	  The horizontal alignment for the text of this text field.
	 */
	Qt::Alignment textAlignment() const;

	/**
	  Whether the text inserted manually in the field (where possible)
	  can be spell-checked.
	 */
	bool canBeSpellChecked() const;

	/**
	  The font size of the text in the form field
	 */
	double getFontSize() const;

	/**
	  Set the font size of the text in the form field (currently only as integer)
	 */
	void setFontSize(int fontSize);

    private:
	Q_DISABLE_COPY(FormFieldText)
    };

    /**
      A form field that represents a choice field.

      \since 0.6
     */
    class POPPLER_QT5_EXPORT FormFieldChoice : public FormField {
    public:

	/**
	   The particular type of this choice field.
	*/
	enum ChoiceType {
	    ComboBox,     ///< A simple singleline text field.
	    ListBox       ///< A multiline text field.
	};

	/// \cond PRIVATE
	FormFieldChoice(DocumentData *doc, ::Page *p, ::FormWidgetChoice *w);
	/// \endcond
	~FormFieldChoice();

	FormType type() const override;

	/**
	  The choice type of the choice field.
	 */
	ChoiceType choiceType() const;

	/**
	  The possible choices of the choice field.
	 */
	QStringList choices() const;

	/**
	  Whether this FormFieldChoice::ComboBox is editable, i.e. the user
	  can type in a custom value.

	  Always false for the other types of choices.
	 */
	bool isEditable() const;

	/**
	  Whether more than one choice of this FormFieldChoice::ListBox
	  can be selected at the same time.

	  Always false for the other types of choices.
	 */
	bool multiSelect() const;

	/**
	  The currently selected choices.
	 */
	QList<int> currentChoices() const;

	/**
	  Sets the selected choices to \p choice.
	 */
	void setCurrentChoices( const QList<int> &choice );
	
	/**
	  The text entered into an editable combo box choice field. Otherwise a null string.
	  
	  \since 0.22
	*/
	QString editChoice() const;
	
	/**
	  Sets the text entered into an editable combo box choice field. Otherwise does nothing.
	  
	  \since 0.22
	*/
	void setEditChoice(const QString& text);

	/**
	  The horizontal alignment for the text of this text field.
	 */
	Qt::Alignment textAlignment() const;

	/**
	  Whether the text inserted manually in the field (where possible)
	  can be spell-checked.

          Returns false if the field is not an editable text field.
	 */
	bool canBeSpellChecked() const;

    private:
	Q_DISABLE_COPY(FormFieldChoice)
    };

    /**
      A signature validation info helper class.

      \since 0.51
     */
    class SignatureValidationInfoPrivate;
    class POPPLER_QT5_EXPORT SignatureValidationInfo {
    public:

	/**
	   The verfication result of the signature.
	*/
	enum SignatureStatus {
	    SignatureValid,          ///< The signature is cryptographically valid.
	    SignatureInvalid,        ///< The signature is cryptographically invalid.
	    SignatureDigestMismatch, ///< The document content was changed after the signature was applied.
	    SignatureDecodingError,  ///< The signature CMS/PKCS7 structure is malformed.
	    SignatureGenericError,   ///< The signature could not be verified.
	    SignatureNotFound,       ///< The requested signature is not present in the document.
	    SignatureNotVerified     ///< The signature is not yet verified.
	};

	/**
	   The verification result of the certificate.
	*/
	enum CertificateStatus {
	    CertificateTrusted,         ///< The certificate is considered trusted.
	    CertificateUntrustedIssuer, ///< The issuer of this certificate has been marked as untrusted by the user.
	    CertificateUnknownIssuer,   ///< The certificate trust chain has not finished in a trusted root certificate.
	    CertificateRevoked,         ///< The certificate was revoked by the issuing certificate authority.
	    CertificateExpired,         ///< The signing time is outside the validity bounds of this certificate.
	    CertificateGenericError,    ///< The certificate could not be verified.
	    CertificateNotVerified      ///< The certificate is not yet verified.
	};

	/**
	    The hash algorithm of the signature
	    \since 0.58
	 */
	enum HashAlgorithm
	{
	    HashAlgorithmUnknown,
	    HashAlgorithmMd2,
	    HashAlgorithmMd5,
	    HashAlgorithmSha1,
	    HashAlgorithmSha256,
	    HashAlgorithmSha384,
	    HashAlgorithmSha512,
	    HashAlgorithmSha224
	};

	/// \cond PRIVATE
	SignatureValidationInfo(SignatureValidationInfoPrivate *priv);
	/// \endcond
	~SignatureValidationInfo();

	/**
	  The signature status of the signature.
	 */
	SignatureStatus signatureStatus() const;

	/**
	  The certificate status of the signature.
	 */
	CertificateStatus certificateStatus() const;

	/**
	  The signer name associated with the signature.
	 */
	QString signerName() const;

	/**
	  The signer subject distinguished name associated with the signature.
	  \since 0.58
	 */
	QString signerSubjectDN() const;

	/**
	  The the hash algorithm used for the signature.
	  \since 0.58
	 */
	HashAlgorithm hashAlgorithm() const;

	/**
	  The signing time associated with the signature.
	 */
	time_t signingTime() const;

	/**
	  Get the signature binary data.
	  \since 0.58
	 */
        QByteArray signature() const;

	/**
	  Get the bounds of the ranges of the document which are signed.
	  \since 0.58
	 */
        QList<qint64> signedRangeBounds() const;

	/**
	  Checks whether the signature authenticates the total document
          except for the signature itself.
          \since 0.58
	 */
        bool signsTotalDocument() const;

	SignatureValidationInfo(const SignatureValidationInfo &other);
	SignatureValidationInfo &operator=(const SignatureValidationInfo &other);

	private:
	Q_DECLARE_PRIVATE(SignatureValidationInfo)

	QSharedPointer<SignatureValidationInfoPrivate> d_ptr;
    };

    /**
      A form field that represents a signature.

      \since 0.51
     */
    class POPPLER_QT5_EXPORT FormFieldSignature : public FormField {
    public:

	/**
	    The types of signature fields.
	    \since 0.58
	*/
	enum SignatureType {
	    AdbePkcs7sha1,
	    AdbePkcs7detached,
	    EtsiCAdESdetached
	};

	/**
	   The validation options of this signature.
	*/
	enum ValidateOptions {
	    ValidateVerifyCertificate = 1, ///< Validate the certificate.
	    ValidateForceRevalidation = 2, ///< Force revalidation of the certificate.
	};

	/// \cond PRIVATE
	FormFieldSignature(DocumentData *doc, ::Page *p, ::FormWidgetSignature *w);
	/// \endcond
	~FormFieldSignature();

	FormType type() const override;

	/**
	    The signature type
	    \since 0.58
	*/
	SignatureType signatureType() const;

	/**
	  Validate the signature with now as validation time.

	  Reset signature validatation info of scoped instance.
	 */
	SignatureValidationInfo validate(ValidateOptions opt) const;

	/**
	  Validate the signature with @p validationTime as validation time.

	  Reset signature validatation info of scoped instance.

	  \since 0.58
	 */
	SignatureValidationInfo validate(int opt, const QDateTime& validationTime) const;

	private:
	Q_DISABLE_COPY(FormFieldSignature)
	};

}

#endif
