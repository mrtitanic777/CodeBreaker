// Tests of the batch queue (batch_queue.hpp): the order files run in, what each row says, files that join, and Cancel all.
#include "app/batch_queue.hpp"

#include "core/testing.hpp"

namespace cb::app::batch {
namespace {

std::vector<std::filesystem::path> bq_files(std::initializer_list<const char*> names) {
    std::vector<std::filesystem::path> out;
    for (const char* n : names) out.push_back(std::filesystem::path("C:/in") / n);
    return out;
}

Item bq_done(const char* folder, std::uint64_t files) {
    Item i;
    i.output = folder;
    i.files = files;
    return i;
}

CB_TEST("app.batch_queue.files_wait_run_in_order_and_each_row_says_where_it_is", [] {
    Queue q;
    CB_CHECK(q.empty() && !q.active() && q.next_waiting() == -1);
    CB_CHECK(q.add(bq_files({"lz32.dll", "ver.dll", "setup.exe"})) == 3);
    CB_CHECK(q.total() == 3 && q.active() && q.next_waiting() == 0);
    CB_CHECK_EQ(q.items()[0].name, "lz32.dll");
    CB_CHECK_EQ(Queue::status_words(q.items()[0]), "Waiting");
    q.start(0);
    CB_CHECK(q.running() && q.next_waiting() == 1);
    q.progress(0, 62, "Decompiling", "17 of 39 functions");
    CB_CHECK_EQ(Queue::status_words(q.items()[0]), "62%");
    CB_CHECK_EQ(q.items()[0].stage, "Decompiling");
    CB_CHECK_EQ(q.items()[0].detail, "17 of 39 functions");
    q.progress(0, 400, "x", "y");   // (a percentage is kept between 0 and 100)
    CB_CHECK(q.items()[0].percent == 100);
    q.progress(1, 50, "x", "y");    // (a file that is not running takes no progress)
    CB_CHECK(q.items()[1].percent == 0);
    q.finish(0, ItemState::Done, bq_done("C:/out/lz32", 12));
    CB_CHECK(!q.running() && q.items()[0].files == 12 && q.items()[0].percent == 100);
    CB_CHECK(q.items()[0].stage.empty());
    CB_CHECK_EQ(Queue::status_words(q.items()[0]), "Done");
    q.start(1);
    Item bad;
    bad.message = "This looks like a PNG image.";
    q.finish(1, ItemState::CannotConvert, bad);
    CB_CHECK_EQ(Queue::status_words(q.items()[1]), "Can't convert");
    CB_CHECK_EQ(q.items()[1].message, "This looks like a PNG image.");
    q.start(2);
    q.finish(2, ItemState::Failed, {});
    CB_CHECK_EQ(Queue::status_words(q.items()[2]), "Failed");
    CB_CHECK(!q.active() && q.next_waiting() == -1);
});

CB_TEST("app.batch_queue.n_of_m_done_the_last_converted_file_and_the_share_of_the_whole", [] {
    Queue q;
    q.add(bq_files({"a", "b", "c", "d"}));
    CB_CHECK_EQ(q.progress_words(), "0 of 4 done");
    CB_CHECK(q.overall_percent() == 0 && q.last_done() == -1);
    q.start(0);
    q.finish(0, ItemState::Done, bq_done("C:/out/a", 1));
    q.start(1);
    q.finish(1, ItemState::CannotConvert, {});
    q.start(2);
    q.progress(2, 50, "Writing", "");
    CB_CHECK_EQ(q.progress_words(), "2 of 4 done");   // (a file that could not be converted has been dealt with)
    CB_CHECK(q.done() == 1 && q.settled() == 2);
    CB_CHECK(q.overall_percent() == (100 + 100 + 50 + 0) / 4);
    q.finish(2, ItemState::Done, bq_done("C:/out/c", 5));
    q.start(3);
    q.finish(3, ItemState::Failed, {});
    CB_CHECK(q.last_done() == 2);   // (the Source card shows c: the last that was converted)
    CB_CHECK(q.done() == 2 && q.overall_percent() == 100);
});

CB_TEST("app.batch_queue.files_dropped_during_a_run_join_the_end", [] {
    Queue q;
    q.add(bq_files({"a", "b"}));
    q.start(0);
    q.add(bq_files({"c"}));
    CB_CHECK(q.total() == 3 && q.items()[2].name == "c");
    q.finish(0, ItemState::Done, bq_done("C:/out/a", 1));
    CB_CHECK(q.next_waiting() == 1);
    q.start(1);
    q.finish(1, ItemState::Done, bq_done("C:/out/b", 1));
    CB_CHECK(q.next_waiting() == 2 && q.active());
    q.start(2);
    q.finish(2, ItemState::Done, bq_done("C:/out/c", 1));
    CB_CHECK(!q.active() && q.progress_words() == "3 of 3 done");
    q.clear();
    CB_CHECK(q.empty());
});

CB_TEST("app.batch_queue.cancel_all_cancels_the_files_still_waiting_and_keeps_the_finished", [] {
    Queue q;
    q.add(bq_files({"a", "b", "c", "d"}));
    q.start(0);
    q.finish(0, ItemState::Done, bq_done("C:/out/a", 1));
    q.start(1);
    q.cancel_waiting();   // (b is running: the worker stops it and finishes it Cancelled)
    CB_CHECK(q.items()[1].state == ItemState::Running);
    CB_CHECK(q.items()[2].state == ItemState::Cancelled && q.items()[3].state == ItemState::Cancelled);
    CB_CHECK(q.next_waiting() == -1);
    q.finish(1, ItemState::Cancelled, {});
    CB_CHECK(!q.active());
    CB_CHECK_EQ(Queue::status_words(q.items()[3]), "Cancelled");
    CB_CHECK(q.done() == 1 && q.last_done() == 0 && q.progress_words() == "4 of 4 done");
});

}  // namespace
}  // namespace cb::app::batch
