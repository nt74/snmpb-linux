## How to localize SnmpB to your language ##

This project is written using the [Qt][] GUI toolkit, and uses
[its excellent support][Qt i18n] for internationalization.

As a translator, you will need a program called [Qt Linguist][]
(or anything else, as long as it understands the TS XML format).

Take the template file **`snmpb_strings.ts`**, open it in Linguist,
do the work, fill out the strings; then **save that file
as `snmpb.aa_BB.ts`** where aa_BB is your target locale identifier
(composed of ISO 639-1 two-letter language code, underscore, then
ISO-3166-1 two-letter country code).

To test: put the translation file under app/l10n, and build project.
Afterwards, run snmpb with `LANG=aa_BB` environment variable set.

Lastly, send your contribution to the maintainers for inclusion.
Direct email is fine for that; you can also file a SourceForge ticket.
Feel free to credit yourself as the translator in the About window.

Thanks in advance!

[Qt]: https://www.qt.io
[Qt i18n]: https://doc.qt.io/qt-5/internationalization.html
[Qt Linguist]: https://doc.qt.io/qt-5/qtlinguist-index.html
