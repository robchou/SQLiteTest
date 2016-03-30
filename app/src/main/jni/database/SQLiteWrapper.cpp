#include <iostream>
#include "SQLiteWrapper.h"
#include "../Log.h"
// TODO: raus
namespace duomai {
    namespace im {

        SQLiteException::SQLiteException(const int error_code,
                                         const char *error_message,
                                         bool delete_message) : error_code_(error_code) {

            error_message_ = sqlite3_mprintf("%s[%d]: %s",
                                             ConvertErrorCodeToString(error_code),
                                             error_code,
                                             error_message ? error_message : "");
            if (delete_message && error_message) {
                sqlite3_free((void*)error_message);
                LOGE("%s", error_message_);
            }

        }

        SQLiteException::SQLiteException(const SQLiteException &exception)
                : error_code_(exception.error_code_) {
            error_message_ = 0;
            if (exception.error_message_) {
                error_message_ = sqlite3_mprintf("%s", exception.error_message_);
                LOGE("%s", error_message_);
            }
        }

        SQLiteException::~SQLiteException() {
            sqlite3_free(error_message_);
            error_message_ = 0;
        }

        int SQLiteException::error_code() const {
            return error_code_;
        }

        char *SQLiteException::error_message() const {
            return error_message_;
        }

        const char *SQLiteException::ConvertErrorCodeToString(int error_code) {
            switch (error_code)
            {
                case SQLITE_OK              : return "SQLITE_OK";
                case SQLITE_ERROR           : return "SQLITE_ERROR";
                case SQLITE_INTERNAL        : return "SQLITE_INTERNAL";
                case SQLITE_PERM            : return "SQLITE_PERM";
                case SQLITE_ABORT           : return "SQLITE_ABORT";
                case SQLITE_BUSY            : return "SQLITE_BUSY";
                case SQLITE_LOCKED          : return "SQLITE_LOCKED";
                case SQLITE_NOMEM           : return "SQLITE_NOMEM";
                case SQLITE_READONLY        : return "SQLITE_READONLY";
                case SQLITE_INTERRUPT       : return "SQLITE_INTERRUPT";
                case SQLITE_IOERR           : return "SQLITE_IOERR";
                case SQLITE_CORRUPT         : return "SQLITE_CORRUPT";
                case SQLITE_NOTFOUND        : return "SQLITE_NOTFOUND";
                case SQLITE_FULL            : return "SQLITE_FULL";
                case SQLITE_CANTOPEN        : return "SQLITE_CANTOPEN";
                case SQLITE_PROTOCOL        : return "SQLITE_PROTOCOL";
                case SQLITE_EMPTY           : return "SQLITE_EMPTY";
                case SQLITE_SCHEMA          : return "SQLITE_SCHEMA";
                case SQLITE_TOOBIG          : return "SQLITE_TOOBIG";
                case SQLITE_CONSTRAINT      : return "SQLITE_CONSTRAINT";
                case SQLITE_MISMATCH        : return "SQLITE_MISMATCH";
                case SQLITE_MISUSE          : return "SQLITE_MISUSE";
                case SQLITE_NOLFS           : return "SQLITE_NOLFS";
                case SQLITE_AUTH            : return "SQLITE_AUTH";
                case SQLITE_FORMAT          : return "SQLITE_FORMAT";
                case SQLITE_RANGE           : return "SQLITE_RANGE";
                case SQLITE_ROW             : return "SQLITE_ROW";
                case SQLITE_DONE            : return "SQLITE_DONE";
                case SQLITE_WRAPPER_ERROR   : return "SQLITE_WRAPPER_ERROR";
                default                     : return "UNKNOWN_ERROR";
            }
        }

        SQLiteWrapper::SQLiteWrapper() : db_(0) {
        }

        bool SQLiteWrapper::Open(std::string const& db_file) {
            if (sqlite3_open(db_file.c_str(), &db_) != SQLITE_OK) {
                LOGE("Open database %s failed", db_file.c_str());
                return false;
            }
            return true;
        }

        bool SQLiteWrapper::SelectStmt(std::string const& stmt, ResultTable& res) {
            char *errmsg;
            int   ret;

            res.reset();

            ret = sqlite3_exec(db_, stmt.c_str(), SelectCallback, static_cast<void*> (&res), &errmsg);

            if (ret != SQLITE_OK) {
                LOGE("%s return %s", __FUNCTION__, SQLiteException::ConvertErrorCodeToString(ret));
                return false;
            }
            return true;
            //  if (ret != SQLITE_OK) {
            //    std::cout << stmt << " [" << errmsg << "]" << std::endl;
            //  }
        }

        // TODO parameter p_col_names
        int SQLiteWrapper::SelectCallback(void *p_data, int num_fields, char **p_fields, char** p_col_names) {
            ResultTable* res = reinterpret_cast<ResultTable*>(p_data);

            ResultRecord record;

#ifdef SQLITE_WRAPPER_REPORT_COLUMN_NAMES
            // Hubert Castelain: column names in the first row of res if res is empty

            if(res->records_.size()==0) {
                ResultRecord col_names;

                for(int i=0; i < num_fields; i++) {
                    if(p_fields[i]) col_names.fields_.push_back (p_col_names[i]);
                    else
                        col_names.fields_.push_back("(null)"); // or what else ?
                }
                res->records_.push_back(col_names);
            }
#endif

            for(int i=0; i < num_fields; i++) {
                // Hubert Castelain: special treatment if null
                if (p_fields[i]) record.fields_.push_back(p_fields[i]);
                else             record.fields_.push_back("<null>");
            }

            res->records_.push_back(record);

            return 0;
        }

        SQLiteStatement* SQLiteWrapper::Statement(std::string const& statement) {
            SQLiteStatement* stmt;
            try {
                stmt = new SQLiteStatement(statement, db_);
                return stmt;
            }
            catch (const char* e) {
                return 0;
            }
        }

        SQLiteStatement::SQLiteStatement(std::string const& statement, sqlite3* db) {
            if ( sqlite3_prepare(
                    db,
                    statement.c_str(),  // stmt
                    -1,                  // If than zero, then stmt is read up to the first nul terminator
                    &stmt_,
                    0                   // Pointer to unused portion of stmt
            )
                 != SQLITE_OK) {
                LOGE("%s : %s", __FUNCTION__, sqlite3_errmsg(db));
            }

            if (!stmt_) {
                LOGE("%s : stmt_ is 0", __FUNCTION__);
            }
        }

        SQLiteStatement::~SQLiteStatement() {
            // Hubert Castelain 28/8/2005
            // Prevent the database remaining locked after some statement.
            // syntax: int sqlite3_finalize(sqlite3_stmt *pStmt);
            if(stmt_) sqlite3_finalize(stmt_);
        }

        SQLiteStatement::SQLiteStatement() :
                stmt_       (0)
        {
        }

        bool SQLiteStatement::Bind(int pos_zero_indexed, std::string const& value) {

            int rc = sqlite3_bind_text (
                    stmt_,
                    pos_zero_indexed+1,  // Index of wildcard
                    value.c_str(),
                    value.length(),      // length of text
                    SQLITE_TRANSIENT     // SQLITE_TRANSIENT: SQLite makes its own copy
            );

            if (rc != SQLITE_OK) {
                LOGE("%s : %s", __FUNCTION__, SQLiteException::ConvertErrorCodeToString(rc));
                return false;
            }
            return true;
        }

        bool SQLiteStatement::Bind(int pos_zero_indexed, double value) {

            int rc = sqlite3_bind_double(
                    stmt_,
                    pos_zero_indexed+1,  // Index of wildcard
                    value
            );
            if (rc != SQLITE_OK) {
                LOGE("%s : %s", __FUNCTION__, SQLiteException::ConvertErrorCodeToString(rc));
                return false;
            }
            return true;
        }

        bool SQLiteStatement::Bind(int pos_zero_indexed, int value) {

            int rc = sqlite3_bind_int(
                    stmt_,
                    pos_zero_indexed+1,  // Index of wildcard
                    value
            );
            if (rc != SQLITE_OK) {
                LOGE("%s : %s", __FUNCTION__, SQLiteException::ConvertErrorCodeToString(rc));
                return false;
            }
            return true;
        }

        bool SQLiteStatement::BindNull(int pos_zero_indexed) {

            int rc = sqlite3_bind_null(
                    stmt_,
                    pos_zero_indexed+1  // Index of wildcard
            );

            if (rc != SQLITE_OK) {
                LOGE("%s : %s", __FUNCTION__, SQLiteException::ConvertErrorCodeToString(rc));
                return false;
            }
            return true;
        }

        bool SQLiteStatement::Execute() {
            int rc = sqlite3_step(stmt_);
            if (rc == SQLITE_BUSY) {
                LOGE("%s : %s", __FUNCTION__, SQLiteException::ConvertErrorCodeToString(rc));
                return false;
            }
            if (rc == SQLITE_ERROR) {
                LOGE("%s : %s", __FUNCTION__, SQLiteException::ConvertErrorCodeToString(rc));
                return false;
            }
            if (rc == SQLITE_MISUSE) {
                LOGE("%s : %s", __FUNCTION__, SQLiteException::ConvertErrorCodeToString(rc));
                return false;
            }
            if (rc != SQLITE_DONE) {
                LOGE("%s : %s", __FUNCTION__, SQLiteException::ConvertErrorCodeToString(rc));
                //sqlite3_reset(stmt_);
                return false;
            }
            sqlite3_reset(stmt_);
            return true;
        }

        SQLiteStatement::dataType SQLiteStatement::DataType(int pos_zero_indexed) {
            return dataType(sqlite3_column_type(stmt_, pos_zero_indexed));
        }

        int SQLiteStatement::ValueInt(int pos_zero_indexed) {
            return sqlite3_column_int(stmt_, pos_zero_indexed);
        }

        std::string SQLiteStatement::ValueString(int pos_zero_indexed) {
            return std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt_, pos_zero_indexed)));
        }

        bool SQLiteStatement::RestartSelect() {
            sqlite3_reset(stmt_);
            return true;
        }

        bool SQLiteStatement::Reset() {
            int rc = sqlite3_step(stmt_);

            sqlite3_reset(stmt_);

            if (rc == SQLITE_ROW) return true;
            return false;
        }

        bool SQLiteStatement::NextRow() {
            int rc = sqlite3_step(stmt_);

            if (rc == SQLITE_ROW   ) {
                return true;
            }
            if (rc == SQLITE_DONE  ) {
                sqlite3_reset(stmt_);
                LOGE("%s : %s", __FUNCTION__, SQLiteException::ConvertErrorCodeToString(rc));
                return false;
            }
            else if (rc == SQLITE_MISUSE) {
                LOGE("%s : %s", __FUNCTION__, SQLiteException::ConvertErrorCodeToString(rc));
            }
            else if (rc == SQLITE_BUSY  ) {
                LOGE("%s : %s", __FUNCTION__, SQLiteException::ConvertErrorCodeToString(rc));
            }
            else if (rc == SQLITE_ERROR ) {
                LOGE("%s : %s", __FUNCTION__, SQLiteException::ConvertErrorCodeToString(rc));
            }
            return false;
        }

        bool SQLiteWrapper::ExecSQL(std::string const &stmt) {
            char *errmsg;
            int   ret;

            ret = sqlite3_exec(db_, stmt.c_str(), 0, 0, &errmsg);

            if(ret != SQLITE_OK) {
                return false;
            }
            return true;

            //if(ret != SQLITE_OK) {
            //  std::cout << stmt << " [" << errmsg << "]" << std::endl;
            //}
        }

        std::string SQLiteWrapper::LastError() {
            return sqlite3_errmsg(db_);
        }

        bool SQLiteWrapper::Begin() {
            return ExecSQL("begin");
        }

        bool SQLiteWrapper::Commit() {
            return ExecSQL("commit");
        }

        bool SQLiteWrapper::Rollback() {
            return ExecSQL("rollback");
        }

    }
}
